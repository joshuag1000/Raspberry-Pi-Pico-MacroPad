#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
// Include TinyUSB libraries.
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

// We are including this for a button option later.
#include "pico/bootrom.h"

// Include the RGB Keypad library.
#include "pico_rgb_keypad.hpp"

//--------------------------------------------------------------------+
// Declarations
//--------------------------------------------------------------------+


enum
{
  REPORT_ID_TINYPICO = 4
};

// We are using this array to store the button assignments the user has setup.
int ButtonAssignments[16][3] = { 0 };

// This array is to be used to store button colours so we can reset the colour after the buttonpress
uint8_t ColourAssignments[16][3] = { 0 };

uint16_t button_states = 0;
uint16_t last_button_states = 0;


// This is all of the tiny usb code with a few modifications pulled from the example at: https://github.com/hathach/tinyusb/blob/master/examples/device/hid_composite/src/main.c
// My code is below.

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, bool ButtonPressed, int KeyCode, int Modifiers)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( ButtonPressed )
      {
        uint8_t keycode[6] = {KeyCode, 0, 0, 0, 0, 0};

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, Modifiers, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( ButtonPressed )
      {
        // volume down
        uint16_t ComsumerKeyCode = KeyCode;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &ComsumerKeyCode, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    default: break;
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1;

  if (next_report_id < REPORT_ID_COUNT)
  {
    if (button_states > 0) {
        send_hid_report(next_report_id, true, 0, 0);
    } else {
        send_hid_report(next_report_id, false, 0, 0);
    }
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms) return;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// Library Code
//--------------------------------------------------------------------+

// This will be used to reference our keypad in our code.
pimoroni::PicoRGBKeypad PicoKeypad;

void RemoveButtonSetup(int ButtonNum) {
    ColourAssignments[ButtonNum][0] = 0;
    ColourAssignments[ButtonNum][1] = 0;
    ColourAssignments[ButtonNum][2] = 0;
    PicoKeypad.illuminate(ButtonNum, 0, 0, 0);
    PicoKeypad.update();
    ButtonAssignments[ButtonNum][0] = 0;
    ButtonAssignments[ButtonNum][1] = 0;
    ButtonAssignments[ButtonNum][2] = 0;
}

// ButtonNum varies from 0 to 15.
void SetupButton(uint8_t ButtonNum, uint8_t r, uint8_t g, uint8_t b, int KeyCode, int ModifierKeys, int KeyboardType) {
    ColourAssignments[ButtonNum][0] = r;
    ColourAssignments[ButtonNum][1] = g;
    ColourAssignments[ButtonNum][2] = b;
    PicoKeypad.illuminate(ButtonNum, r, g, b);
    PicoKeypad.update();
    ButtonAssignments[ButtonNum][0] = KeyCode;
    ButtonAssignments[ButtonNum][1] = ModifierKeys;
    ButtonAssignments[ButtonNum][2] = KeyboardType;
}


// Just a simple function to be called that allows us to setup the TinyUSB
void InitializeDevice(void) {
    // init the keypad
    PicoKeypad.init();
    PicoKeypad.set_brightness(1.0f);
    board_init();
    tusb_init();
}



bool TimerCancelled = true;
int64_t ResetLEDsRepeat(alarm_id_t id, void *user_data)
{
    for (int i = 0; i <= 15; i++)
    {
        PicoKeypad.illuminate(i, ColourAssignments[i][0], ColourAssignments[i][1], ColourAssignments[i][2]);
    }
    PicoKeypad.update();
    TimerCancelled = true;
    return 0;
}

/* 
This will be added into the loop for void main this function will handle the detection of the button presses and handles the keypress too.
Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
tud_hid_report_complete_cb() is used to send the next report after previous one is complete
*/
void MacropadLoop(void) {
    tud_task(); // tinyusb device task
    led_blinking_task();
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if ( board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    button_states = PicoKeypad.get_button_states();

    // Remote wakeup
    if ( tud_suspended() && (button_states > 0) )
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }else
    {
        // Check to see if a button was pressed and that it was not the same as the last check. Allows us to avoid duplicate registers.
        if (last_button_states != button_states && button_states)
        {
            last_button_states = button_states;
            if (button_states)
            { 
                uint8_t ButtonLEDAddr = 0;
                // convert the number into the 0 - 15 address for the led. Uses a binary shift to perform this calcualtion.
                unsigned int number = button_states;
                while (number >>= 1)
                    ++ButtonLEDAddr;

                // If the user has specified our custom keyboard then we need to make sure that we handle it.
                if (ButtonAssignments[ButtonLEDAddr][2] == REPORT_ID_TINYPICO) { 
                    for (uint8_t i = 0; i < 16; i++)
                    {
                        PicoKeypad.illuminate(i, 0x00, 0x00, 0x00);
                    }
                    PicoKeypad.update();
                    reset_usb_boot(0, 0);
                // Check to see if the keypad is actually ready for a keypress
                }else if (tud_hid_ready())
                {
                    // Check if the system is ready and if it isn't show a red key
                    PicoKeypad.illuminate(ButtonLEDAddr, 0x20, 0x20, 0x00);
                    send_hid_report(ButtonAssignments[ButtonLEDAddr][2], true, ButtonAssignments[ButtonLEDAddr][0], ButtonAssignments[ButtonLEDAddr][1]);
                }
                else
                {
                    for (int i = 0; i <= 15; i++)
                    {
                        PicoKeypad.illuminate(i, 0x20, 0x00, 0x00);
                    }
                }
                PicoKeypad.update();

                // Start the timer to reset the colours after 300ms.
                if (TimerCancelled == true)
                {
                    TimerCancelled = false;
                    add_alarm_in_ms(300, ResetLEDsRepeat, NULL, false);
                }
            }
        }
    }

}
