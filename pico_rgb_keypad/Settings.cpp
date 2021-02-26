#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "pico/stdlib.h"
#include <string.h>

// TINYUSB LIBRARIES
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

//#include "pico/stdlib.h"
//#include "pico/multicore.h"

#include "Main-Code.h"

// Set this value to true if you are using an ir reciever with the keypad
bool UseIR() {
    return false;
}

// Set this to true if you want the TinyUSB blinking task to run
bool TinyUsbBinkingTask() {
    return false;
}

void DefaultColours()
{
    // these are intentionally defined individually to allow the default colours of the button to be specified individually - White: 0x05, 0x05, 0x05
    IlluminateKeypad(0, 0x00, 0x00, 0x20);
    IlluminateKeypad(1, 0x00, 0x20, 0x00);
    IlluminateKeypad(2, 0x20, 0x05, 0x20);
    IlluminateKeypad(3, 0x20, 0x00, 0x00);

    IlluminateKeypad(4, 0x00, 0x00, 0x20);
    IlluminateKeypad(5, 0x00, 0x20, 0x00);
    IlluminateKeypad(6, 0x20, 0x00, 0x20);
    IlluminateKeypad(7, 0x20, 0x00, 0x20);

    IlluminateKeypad(8, 0x00, 0x00, 0x20);
    IlluminateKeypad(9, 0x05, 0x05, 0x05);
    IlluminateKeypad(10, 0x05, 0x05, 0x05);
    IlluminateKeypad(11, 0x05, 0x05, 0x05);

    IlluminateKeypad(12, 0x05, 0x05, 0x05);
    IlluminateKeypad(13, 0x05, 0x05, 0x05);
    IlluminateKeypad(14, 0x05, 0x05, 0x05);
    IlluminateKeypad(15, 0x00, 0x20, 0x20);

    UpdateKeypad();
}

void ButtonDown(int buttonValue)
{
    // Send those beautiful keyboard commands!
    switch (buttonValue)
    {
    case 0:
        // PRESS THE KEY!!!!!
        // CTRL + SHIFT + O. Calles twice because it is a toggle
        PressKey(HID_KEY_O, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, false);
        break;
    
    case 1:
        // CTRL + UP_ARROW. Calles twice because it is a toggle
        PressKey(HID_KEY_ARROW_UP, KEYBOARD_MODIFIER_LEFTCTRL, false);
        break;

    case 2:
        // Play/Pause Media Key
        PressKey(HID_USAGE_CONSUMER_PLAY_PAUSE, 0, true);
        break;

    case 3:
        // CTRL + ALT + Num0. Calles twice because it is a toggle
        PressKey(HID_KEY_KEYPAD_0, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTALT, false);
        break;

    case 4:
        // CTRL + SHIFT + M. Calles twice because it is a toggle
        PressKey(HID_KEY_M, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, false);
        break;

    case 5:
        // CTRL + DOWN_ARROW. Calles twice because it is a toggle
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
        // CTRL + SHIFT + K. Calles twice because it is a toggle
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
        // Windows + L. Calles twice because it is a toggle
        PressKey(HID_KEY_L, KEYBOARD_MODIFIER_LEFTGUI, false);
        break;

    default:
        break;
    }
    
}

// This can be ignored if the IR sensor is disabled
void IRRecieveCode(int IRCode) 
{

}