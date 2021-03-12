// TINYUSB LIBRARIES
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

// Predefines the press key function
void PressKey(int Keycode, int ModifierKeys, bool MediaKey);

// include the RGB keypad's config file.
#include "pico_rgb_keypad.hpp"

using namespace pimoroni;
extern PicoRGBKeypad pico_keypad;

// Allows the user to set the duration before the LED's will dim again
int DimLedDuration = 300000;
// Set this to true if you want the TinyUSB blinking task to run
bool TinyUsbBinkingTask = false;
// Set this value to true if you are using an ir reciever with the keypad
bool UseIR = false;

void DefaultColours()
{
    // these are intentionally defined individually to allow the default colours of the button to be specified individually - White: 0x05, 0x05, 0x05
    // WARNING I RECOMMEND THAT YOU DO NOT SET THE COLOUR CODE HIGHER THAN 0x20 AS THE LEDS WILL GET VERY HOT.
    pico_keypad.illuminate(0, 0x00, 0x00, 0x20);
    pico_keypad.illuminate(1, 0x00, 0x20, 0x00);
    pico_keypad.illuminate(2, 0x20, 0x05, 0x20);
    pico_keypad.illuminate(3, 0x20, 0x00, 0x00);

    pico_keypad.illuminate(4, 0x00, 0x00, 0x20);
    pico_keypad.illuminate(5, 0x00, 0x20, 0x00);
    pico_keypad.illuminate(6, 0x20, 0x00, 0x20);
    pico_keypad.illuminate(7, 0x20, 0x00, 0x20);

    pico_keypad.illuminate(8, 0x00, 0x00, 0x20);
    pico_keypad.illuminate(9, 0x05, 0x05, 0x05);
    pico_keypad.illuminate(10, 0x05, 0x05, 0x05);
    pico_keypad.illuminate(11, 0x05, 0x05, 0x05);

    pico_keypad.illuminate(12, 0x05, 0x05, 0x05);
    pico_keypad.illuminate(13, 0x05, 0x05, 0x05);
    pico_keypad.illuminate(14, 0x05, 0x05, 0x05);
    pico_keypad.illuminate(15, 0x00, 0x20, 0x20);

    pico_keypad.update();
}

// Notes for the PressKey Function.
// The bool parameter must be set to true if the key comes from the consumer keyboard. And false for the normal keyboard.

void ButtonDown(int buttonValue)
{
    // Send those beautiful keyboard commands!
    switch (buttonValue)
    {
    case 0:
        // CTRL + SHIFT + O.
        PressKey(HID_KEY_O, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, false);
        break;

    case 1:
        // CTRL + UP_ARROW.
        PressKey(HID_KEY_ARROW_UP, KEYBOARD_MODIFIER_LEFTCTRL, false);
        break;

    case 2:
        // Play/Pause Media Key
        PressKey(HID_USAGE_CONSUMER_PLAY_PAUSE, 0, true);
        break;

    case 3:
        // CTRL + ALT + Num0.
        PressKey(HID_KEY_KEYPAD_0, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTALT, false);
        break;

    case 4:
        // CTRL + SHIFT + M.
        PressKey(HID_KEY_M, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, false);
        break;

    case 5:
        // CTRL + DOWN_ARROW.
        PressKey(HID_KEY_ARROW_DOWN, KEYBOARD_MODIFIER_LEFTCTRL, false);
        break;

    case 6:
        // Previous Media Key.
        PressKey(HID_USAGE_CONSUMER_SCAN_PREVIOUS, 0, true);
        break;

    case 7:
        // Next Media Key
        PressKey(HID_USAGE_CONSUMER_SCAN_NEXT, 0, true);
        break;

    case 8:
        // CTRL + SHIFT + K.
        PressKey(HID_KEY_K, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, false);
        break;

    case 9:

        break;

    case 10:

        break;

    case 11:

        break;

    case 12:

        break;

    case 13:

        break;

    case 14:

        break;

    case 15:
        // Windows + L.
        PressKey(HID_KEY_L, KEYBOARD_MODIFIER_LEFTGUI, false);
        break;

    default:
        break;
    }
}

// This can be ignored if the IR sensor is disabled. DO NOT DELETE AS IT WILL BREAK THE CODE.
void IRRecieveCode(int IRCode)
{
    
}

extern bool has_consumer_key;
extern bool has_key;

// PressKey Allows a key to be easily pressed. This can be used with Media keys and normal keyboard keys.
void PressKey(int Keycode, int ModifierKeys, bool MediaKey)
{
    if (MediaKey == true)
    {
        uint16_t CodeToUse = Keycode;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &CodeToUse, 2);
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &CodeToUse, 2);
        has_consumer_key = true;
    }
    else
    {
        uint8_t CodeToUse[6] = {Keycode, 0, 0, 0, 0, 0};
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, ModifierKeys, CodeToUse);
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, ModifierKeys, CodeToUse);
        has_key = true;
    }
}