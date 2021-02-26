#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "pico/stdlib.h"
#include <string.h>

// TINYUSB LIBRARIES
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

// include my settings file
#include "Settings.h"

#include "pico_rgb_keypad.hpp"

using namespace pimoroni;

PicoRGBKeypad pico_keypad;

void IlluminateKeypad(int LED, int R, int G, int B) {
    pico_keypad.illuminate(LED, R, G, B);
}

void UpdateKeypad() {
    pico_keypad.update();
}

//--------------------------------------------------------------------+
// MY CODE
//--------------------------------------------------------------------+

// predeclaring
void ButtonDown(int buttonValue);
void IRRecieveCode(int RCode);
//int PressKey(uint8_t keycode[6]);

//--------------------------------------------------------------------+
// CODE FOR USB TINY
//--------------------------------------------------------------------+

enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void KeyboardGo();
void InitIR() ;

//struct repeating_timer timer;
bool TimerCancelled = true;
int64_t ResetLEDsRepeat(alarm_id_t id, void *user_data)
{
    DefaultColours();
    TimerCancelled = true;
    return 0;
}

struct repeating_timer timer;
bool LEDDimClock = false;
bool DimLEDTimer(struct repeating_timer *t) {
    pico_keypad.set_brightness(0.2f);
    pico_keypad.update();
    LEDDimClock = false;
    cancel_repeating_timer(&timer);
    return true;
}

uint16_t last_button_states = 0;

int main()
{
    //init the serial over usb
    //stdio_init_all();

    // init the keypad
    pico_keypad.init();
    pico_keypad.set_brightness(1.0f);

    DefaultColours();

    // init tiny usb
    board_init();
    tusb_init();
    if (UseIR() == true){
        InitIR();
    }

    add_repeating_timer_ms(300000, DimLEDTimer, NULL, &timer);
    LEDDimClock = true;

    while (true)
    {
        if (TinyUsbBinkingTask() == true)
        {
            led_blinking_task();
        }

        KeyboardGo();

        tud_task(); // tinyusb device task
    }

    return 0;
}

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
    (void)remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// use to avoid send multiple consecutive zero report for keyboard
static bool has_key = false;
// use to avoid send multiple consecutive zero report
static bool has_consumer_key = false;

// PressKey Allows a key to be easily pressed. This can be used with Media keys and normal keyboard keys.
void PressKey(int Keycode, int ModifierKeys, bool MediaKey)
{
    if (MediaKey == true) {
        uint16_t CodeToUse = Keycode;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &CodeToUse, 2);
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &CodeToUse, 2);
        has_consumer_key = true;
    } else
    {
        uint8_t CodeToUse[6] = {Keycode, 0, 0, 0, 0, 0};
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, ModifierKeys, CodeToUse);
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, ModifierKeys, CodeToUse);
        has_key = true;
    }
    
}

// This will be used to initialise the IR sensor for when there is one.
void InitIR() 
{

}

// This will toggle the inputted key on the keyboard.
void KeyboardGo()
{
    // Init USB
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;
    //uint32_t const btn = 1;

    // Remote wakeup
    if (tud_suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }

    /*------------- Keyboard -------------*/
    if (tud_hid_ready())
    {
        static bool toggle = false;
        if (toggle = !toggle)
        {
            // read button states from i2c expander. Then pass the values to the handler which will execute the button presses.
            uint16_t button_states = pico_keypad.get_button_states();

            if (last_button_states != button_states && button_states)
            {
                last_button_states = button_states;
                if (button_states)
                {
                    // convert the number into the 0 - 15 address for the led. Uses a binary shift to perform this calcualtion.
                    unsigned int number = button_states;
                    int ButtonLEDAddr = 0;
                    while (number >>= 1)
                        ++ButtonLEDAddr;

                    if (LEDDimClock == true) {
                        cancel_repeating_timer(&timer);
                        LEDDimClock = false;
                    } else {
                        pico_keypad.set_brightness(1.0f);
                        pico_keypad.update();
                    }

                    // make the colour of the button pressed go yellow temporarily
                    if (ButtonLEDAddr <= 15)
                    {
                        pico_keypad.illuminate(ButtonLEDAddr, 0x20, 0x20, 0x00);
                        pico_keypad.update();
                        if (TimerCancelled == true)
                        {
                            TimerCancelled = false;
                            add_alarm_in_ms(300, ResetLEDsRepeat, NULL, false);
                        }
                    }   
                    ButtonDown(ButtonLEDAddr);
                    add_repeating_timer_ms(300000, DimLEDTimer, NULL, &timer);
                    LEDDimClock = true;
                }
            }
            last_button_states = button_states;

            // Future addition - Add IR
            int IRCode = 0;
            // Check here to see if there has been a new IR code recieved.
            bool IRRecievedCode = false;

            // TO ADD - IR CODE

            if (IRRecievedCode == true && UseIR() == true)
            {
                IRRecieveCode(IRCode);
            }
            // ----------------------
        }
        else
        {
            // send empty key report if previously has key pressed
            if (has_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            uint16_t empty_key = 0;
            if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
            has_consumer_key = false;
            has_key = false;
        }
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// Making TinyUSB a little easier to use
//--------------------------------------------------------------------+

