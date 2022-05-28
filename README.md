# Raspberry-Pi-Pico-MacroPad
[![Compile For the Pico.](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml/badge.svg)](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/actions/workflows/main.yml)<br>
This code allows you to use the Pimoroni RGB Keypad for the Pico as a macro keypad in C++ <br>
Setting what keybind each key is really easy to do and the code is written in a way that makes it easy to change what each key does.<br>
It is advised that you clone the latest version of the code when you want to use this project rather than using the latest release. The latest release might not contain the latest version of the code. <br>
The latest version of the code can be downloaded here: [Download](https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/archive/refs/heads/main.zip)

<br>
This code uses the TinyUSB library in order to perform the hid keyboard responses. <br>
To find the possible keys that can be used visit here: [TinyUSB hid.h](https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h) <br>
ONLY KEYS FROM THE NORMAL KEYBOARD AND THE CONSUMER KEYBOARD are supported by default you will have to add any other keyboards yourself. Modifier keys such as CTRL and ALT can only be used with the normal keyboard.<br>
The Code cannot be published pre-compiled because all of the keybinds are set in the code. <br>
<br>
If you run into any bugs please open an issue.

## Building
For building see: https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/blob/730b8ce94454ee2b8776899c23bb157c8112fe9c/.github/workflows/main.yml#L30-L51 <br>
There you will find examples of how to build the code and quickly setup the pico SDK if you have not done that already.

## To Do:
 - Attempt to get scroll lock led working

# Links
[The Pi Hut - Raspberry Pi Pico](https://thepihut.com/products/raspberry-pi-pico) <br>
[Pimoroni - Raspberry Pi Pico](https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227) <br>
[Pimoroni - RGB Keypad Base](https://shop.pimoroni.com/products/pico-rgb-keypad-base) <br>

# Testing
This has been tested on: <br>
 - Raspberry Pi Pico: [Pimoroni - Raspberry Pi Pico](https://shop.pimoroni.com/products/raspberry-pi-pico?variant=32402092294227), [The Pi Hut - Raspberry Pi Pico](https://thepihut.com/products/raspberry-pi-pico)
 - With the Pimoroni RGB Keypad Base: https://shop.pimoroni.com/products/pico-rgb-keypad-base 
 - Compiled on Arch Linux and Ubuntu 20.04 LTS using the latest pico-sdk, Pimoroni Libraries on 24/12/2021

