#  The MIT License (MIT)
#
#  Copyright (c) 2022 Joshua Glass (SuperNinja_4965)
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# Define our project
project(Pico-MacroPad C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Check to see if TinyUSB has been included in the SDK
if (TARGET tinyusb_device)
  # Import the pimoroni drivers and libraries
  add_subdirectory(libs/pimoroni-pico/drivers)
  add_subdirectory(libs/pimoroni-pico/libraries/pico_rgb_keypad)

  # Add our binary
  add_executable(
    Pico-MacroPad
    src/Main.cpp
    src/RGBMacroLibrary.cpp
    src/usb_descriptors.c
  )

  # Make sure TinyUSB can find tusb_config.h
  target_include_directories(Pico-MacroPad PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src)

  # Set the Pico program information
  pico_set_program_name(Pico-MacroPad "Raspberry Pi Pico MacroPad")
  pico_set_program_version(Pico-MacroPad "2022-05-2")

  # Pull in pico libraries that we need
  target_link_libraries(Pico-MacroPad PUBLIC pico_stdlib pico_rgb_keypad tinyusb_device tinyusb_board)

  # create map/bin/hex file etc.
  pico_add_extra_outputs(Pico-MacroPad)

elseif(PICO_ON_DEVICE)
    message(WARNING "Not building Pico-MacroPad because TinyUSB submodule is not initialized in the SDK")
endif()
