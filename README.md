# Raspberry-Pi-Pico-MacroPad
This code allows you to use the Pimoroni RGB Keypad for the Pico as a macro keypad in C++

To edit the keybinds for the Pimoroni RGB Keypad go to the Settings.cpp file.<br>
The Project does not have support for an IR sensor at this time but I will add support for it later. <br>
<br>
To find the possible keys that can be used visit here: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h <br>
NLY KEYS FROM THE NORMAL KEYBOARD AND THE CONSUMER KEYBOARD are supported. Modifier keys such as CTRL and ALT can only be used with the normal keyboard.<br>

The Code cannot be published pre-compiled because all of the keybinds are set in the code. <br>