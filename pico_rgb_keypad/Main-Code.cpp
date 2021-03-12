#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// TINYUSB LIBRARIES
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

// include my settings file
#include "Settings.h"

// defining external settings so they can be used
extern int DimLedDuration;
extern bool TinyUsbBinkingTask;
extern bool UseIR;

// include the RGB keypad's config file.
#include "pico_rgb_keypad.hpp"

using namespace pimoroni;
PicoRGBKeypad pico_keypad;

//--------------------------------------------------------------------+
// CODE FOR USB TINY - Generic code for TinyUSB
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
void InitIR();

// This timer is used to reset the colours on the LEDS when a button is pressed (when a button is pressed it's colour is changed to yellow)
bool TimerCancelled = true;
int64_t ResetLEDsRepeat(alarm_id_t id, void *user_data)
{
    DefaultColours();
    TimerCancelled = true;
    return 0;
}

// simple timer that will DIM the led's when called. The timer is started below.
struct repeating_timer timer;
bool LEDDimClock = false;
bool DimLEDTimer(struct repeating_timer *t)
{
    pico_keypad.set_brightness(0.2f);
    pico_keypad.update();
    LEDDimClock = false;
    cancel_repeating_timer(&timer);
    return true;
}

uint16_t last_button_states = 0;

int main()
{
    // init the keypad
    pico_keypad.init();
    pico_keypad.set_brightness(1.0f);

    // Sets the default Colours
    DefaultColours();

    // init tiny usb
    board_init();
    tusb_init();
    // Inits the IR (when there is code for that) if the IR option is enabled.
    if (UseIR == true)
    {
        InitIR();
    }

    add_repeating_timer_ms(DimLedDuration, DimLEDTimer, NULL, &timer);
    LEDDimClock = true;

    while (true)
    {
        if (TinyUsbBinkingTask == true)
        {
            led_blinking_task();
        }

        // This is where the checking to see if a button is pressed and if the IR sensor has recieved a code.
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
bool has_key = false;
// use to avoid send multiple consecutive zero report
bool has_consumer_key = false;

// This will be used to initialise the IR sensor for when there is one.
void InitIR()
{
}

struct UpdateReturnStruct
{
    bool Check;
    int Num;
};

UpdateReturnStruct UpdateButtons()
{
    /*------------- Checking sensors and pressing keys -------------*/
    // read button states from i2c expander. Also handles any colours for the buttons and sets the KeypadButtonPressed to true
    uint16_t button_states = pico_keypad.get_button_states();
    bool ButtonPressed = false;
    int ButtonLEDAddr = 0;

    if (last_button_states != button_states && button_states)
    {
        last_button_states = button_states;
        if (button_states)
        {
            // convert the number into the 0 - 15 address for the led. Uses a binary shift to perform this calcualtion.
            unsigned int number = button_states;
            while (number >>= 1)
                ++ButtonLEDAddr;

            // if the timer that dims the leds after 5 mins is running cancel it. if it isn't then reset the led brightness
            if (LEDDimClock == true)
            {
                cancel_repeating_timer(&timer);
                LEDDimClock = false;
            }
            else
            {
                pico_keypad.set_brightness(1.0f);
                pico_keypad.update();
            }

            // make the colour of the button pressed go yellow temporarily. Or red if USB is not ready.
            if (ButtonLEDAddr <= 15)
            {
                if (tud_hid_ready())
                {
                    // Check if the system is ready and if it isn't show a red key
                    pico_keypad.illuminate(ButtonLEDAddr, 0x20, 0x20, 0x00);
                }
                else
                {
                    for (int i = 0; i <= 15; i++) {
                        pico_keypad.illuminate(i, 0x20, 0x00, 0x00);
                    }
                }
                pico_keypad.update();

                // Start the timer to reset the colours after 300ms
                if (TimerCancelled == true)
                {
                    TimerCancelled = false;
                    add_alarm_in_ms(300, ResetLEDsRepeat, NULL, false);
                }
            }

            // set the button pressed check to true.
            ButtonPressed = true;

            // restart the timer that will dim the leds after 5 mins
            add_repeating_timer_ms(DimLedDuration, DimLEDTimer, NULL, &timer);
            LEDDimClock = true;
        }
    }
    last_button_states = button_states;

    UpdateReturnStruct Results = {ButtonPressed, ButtonLEDAddr};
    return Results;
}

UpdateReturnStruct UpdateIR()
{
    // Check here to see if there has been a new IR code recieved.
    bool IRRecievedCode = false;
    int IRCode = -1;

    // TO ADD - IR CODE (fetch IR Code if there is one)

    // ----------------------
    UpdateReturnStruct Results = {IRRecievedCode, IRCode};
    return Results;
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

    UpdateReturnStruct CheckButtons = UpdateButtons();
    UpdateReturnStruct CheckIR = UpdateIR();

    // call the function in the settings file to run the code that will press the key.
    if (tud_hid_ready())
    {
        if (CheckButtons.Check == true || CheckIR.Check == true)
        {
            if (CheckButtons.Check == true)
                ButtonDown(CheckButtons.Num);

            if (CheckIR.Check == true && UseIR)
                IRRecieveCode(CheckIR.Num);
        }
        else
        {
            // send empty key report if previously has key pressed
            if (has_key)
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_key = false;
            uint16_t empty_key = 0;
            if (has_consumer_key)
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
            has_consumer_key = false;
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