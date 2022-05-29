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

cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(rgb_keypad_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

function(add_resource target file)
  get_filename_component(NAME ${ARGV1} NAME_WE)
  set(FILENAME ${ARGV1})

  add_custom_command(
    OUTPUT ${NAME}.o

    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
            ${CMAKE_CURRENT_BINARY_DIR}

    COMMAND arm-none-eabi-ld -r -b binary -o ${NAME}.o ${FILENAME}
    DEPENDS ${FILENAME}
  )

  target_sources(${ARGV0} PRIVATE ${NAME}.o)
endfunction(add_resource)

include(example_auto_set_url.cmake)

add_subdirectory(libs/pimoroni-pico/drivers)
add_subdirectory(libs/pimoroni-pico/libraries/pico_rgb_keypad)

# Add our binaries
add_subdirectory(src)
