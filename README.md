# Raspberry-Pi-Pico-MacroPad
[![Compile For the Pico.](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml/badge.svg)](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml)<br>
This code allows you to use the Pimoroni RGB Keypad for the Pico as a macro keypad in C++

To edit the keybinds for the Pimoroni RGB Keypad go to the Settings.cpp file.<br>
The Project does not have support for an IR sensor at this time but I will add support for it later. <br>
<br>
To find the possible keys that can be used visit here: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h <br>
ONLY KEYS FROM THE NORMAL KEYBOARD AND THE CONSUMER KEYBOARD are supported by default you will have to add any other keyboards yourself. Modifier keys such as CTRL and ALT can only be used with the normal keyboard.<br>
The Code cannot be published pre-compiled because all of the keybinds are set in the code. <br>

There are some bugs in the code. They will be worked out when i have time

## To Do:
Add IR support<br>
Get the status of num/caps/scroll lock keys and set the colours of those buttons depending on their status
<br>
<br>
# Links
https://thepihut.com/products/raspberry-pi-pico <br>
https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227 <br>
https://shop.pimoroni.com/products/pico-rgb-keypad-base <br>
