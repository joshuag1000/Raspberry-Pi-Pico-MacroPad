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

int main() {
    // Ready the device
    InitializeDevice();

    // Setup our keys
    SetupButton(0, 0x00, 0x00, 0x20, HID_KEY_O, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    SetupButton(1, 0x00, 0x20, 0x00, HID_KEY_ARROW_UP, KEYBOARD_MODIFIER_LEFTCTRL, REPORT_ID_KEYBOARD);
    SetupButton(2, 0x20, 0x05, 0x20, HID_USAGE_CONSUMER_PLAY_PAUSE, 0, REPORT_ID_CONSUMER_CONTROL);
    SetupButton(3, 0x20, 0x00, 0x00, HID_KEY_KEYPAD_0, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTALT, REPORT_ID_KEYBOARD);

    SetupButton(4, 0x00, 0x00, 0x20, HID_KEY_M, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    //SetupButton(5, 0x00, 0x00, 0x20, HID_KEY_M, KEYBOARD_MODIFIER_LEFTCTRL + KEYBOARD_MODIFIER_LEFTSHIFT, REPORT_ID_KEYBOARD);
    //RemoveButtonSetup(ButtonNum); // Use this to remove a buttons config

    while (true)
    {
        // Run the libraries loop to handle keypresses.
        MacropadLoop();
    }
    
    return 0;
}
