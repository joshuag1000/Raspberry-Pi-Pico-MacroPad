# Raspberry-Pi-Pico-MacroPad
[![Compile For the Pico.](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml/badge.svg)](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml)<br>
This code allows you to use the Pimoroni RGB Keypad for the Pico as a macro keypad in C++ <br>
Setting what keybind each key is really easy to do and the code is written in a way that makes it easy to change what each key does.<br>

<br>
This code uses the TinyUSB library in order to perform the hid keyboard responses.
To find the possible keys that can be used visit here: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h <br>
ONLY KEYS FROM THE NORMAL KEYBOARD AND THE CONSUMER KEYBOARD are supported by default you will have to add any other keyboards yourself. Modifier keys such as CTRL and ALT can only be used with the normal keyboard.<br>
The Code cannot be published pre-compiled because all of the keybinds are set in the code. <br>

If you run into any bugs please open an issue.

## To Do:
 - Attempt to get scroll lock led working

# Links
https://thepihut.com/products/raspberry-pi-pico <br>
https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227 <br>
https://shop.pimoroni.com/products/pico-rgb-keypad-base <br>

# Testing
This has been tested on: <br>
 - Raspberry Pi Pico: https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227 https://thepihut.com/products/raspberry-pi-pico
 - With the Pimoroni RGB Keypad Base: https://shop.pimoroni.com/products/pico-rgb-keypad-base 
 - Compiled on Fedora 35 and Ubuntu 20.04 LTS using the latest pico-sdk, Pimoroni Libraries on 24/12/2021
