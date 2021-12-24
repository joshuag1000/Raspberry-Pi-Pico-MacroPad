# Raspberry-Pi-Pico-MacroPad
[![Compile For the Pico.](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml/badge.svg)](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml)<br>
This code allows you to use the Pimoroni RGB Keypad for the Pico as a macro keypad in C++ <br>
Setting what keybind each key is really easy to do and the code is written in a way that makes it easy to change what each key does.<br>
There will still be bugs at this point and I am trying my best to get the code working again.<br>

<br>
To find the possible keys that can be used visit here: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h <br>
ONLY KEYS FROM THE NORMAL KEYBOARD AND THE CONSUMER KEYBOARD are supported by default you will have to add any other keyboards yourself. Modifier keys such as CTRL and ALT can only be used with the normal keyboard.<br>
The Code cannot be published pre-compiled because all of the keybinds are set in the code. <br>

There are some bugs in the code. They will be worked out when i have time

## To Do:
 - Add dim timer
 - Make Blinking task optional
 - Add Caps, Num and scroll lock led status options.

# Links
https://thepihut.com/products/raspberry-pi-pico <br>
https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227 <br>
https://shop.pimoroni.com/products/pico-rgb-keypad-base <br>
