#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Include TinyUSB libraries. We do this so we can easily reference our tinyusb keyboard keybinds.
#include "tusb.h"
#include "bsp/board.h"
// Used for specifying our keyboard type.
#include "RGBMacroLibrary.hpp"

enum
{
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_TINYPICO = 4
};


//------------------------------------------------------------------------------------------------------------------------+
// Useful info:
// The RGBMacroLibrary uses 2 of the RP2040's timers.
// To find out what keys work you can look here: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h
// Any keys in the REPORT_ID_KEYBOARD and REPORT_ID_CONSUMER_CONTROL devices will work.
//------------------------------------------------------------------------------------------------------------------------+

// Code enters here.
int main() {
    // Ready the device
    InitializeDevice();

    // Setup our keys These are just the keys I personally use.
    SetupButton(0, 0x00, 0x00, 0x20, HID_KEY_O, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    SetupButton(1, 0x00, 0x20, 0x00, HID_KEY_ARROW_UP, KEYBOARD_MODIFIER_LEFTCTRL, REPORT_ID_KEYBOARD);
    SetupButton(2, 0x20, 0x05, 0x20, HID_USAGE_CONSUMER_PLAY_PAUSE, 0, REPORT_ID_CONSUMER_CONTROL);
    SetupButton(3, 0x20, 0x00, 0x00, HID_KEY_KEYPAD_0, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTALT, REPORT_ID_KEYBOARD);

    SetupButton(4, 0x00, 0x00, 0x20, HID_KEY_M, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    SetupButton(5, 0x00, 0x20, 0x00, HID_KEY_ARROW_DOWN, KEYBOARD_MODIFIER_LEFTCTRL, REPORT_ID_KEYBOARD);
    SetupButton(6, 0x20, 0x00, 0x20, HID_USAGE_CONSUMER_SCAN_PREVIOUS, 0, REPORT_ID_CONSUMER_CONTROL);
    SetupButton(7, 0x20, 0x00, 0x20, HID_USAGE_CONSUMER_SCAN_NEXT, 0, REPORT_ID_CONSUMER_CONTROL);

    SetupButton(8, 0x00, 0x00, 0x20, HID_KEY_K, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    SetupButton(9, 0x00, 0x20, 0x20, HID_KEY_A, KEYBOARD_MODIFIER_LEFTGUI + KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT + KEYBOARD_MODIFIER_LEFTALT, REPORT_ID_KEYBOARD);
    SetupButton(10, 0x00, 0x00, 0x20, HID_KEY_D, KEYBOARD_MODIFIER_LEFTGUI + KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT + KEYBOARD_MODIFIER_LEFTALT, REPORT_ID_KEYBOARD);
    SetupButton(11, 0x20, 0x00, 0x00, 0, 0, REPORT_ID_TINYPICO);

    SetupButton(12, 0x00, 0x00, 0x00, HID_KEY_CAPS_LOCK, 0, REPORT_ID_KEYBOARD);
    SetupButton(13, 0x00, 0x00, 0x00, HID_KEY_NUM_LOCK, 0, REPORT_ID_KEYBOARD);
    SetupButton(14, 0x00, 0x00, 0x00, HID_KEY_SCROLL_LOCK, 0, REPORT_ID_KEYBOARD);
    SetupButton(15, 0x00, 0x20, 0x20, HID_KEY_L, KEYBOARD_MODIFIER_LEFTGUI, REPORT_ID_KEYBOARD);
    //RemoveButtonSetup(ButtonNum); // Use this to remove a buttons config.

    while (true)
    {
        // Run the libraries loop to handle keypresses. False disables the blinking task, 300000 is the sleep timer duration in ms.
        MacropadLoop(false, 300000);
    }
    
    return 0;
}
