add_executable(
  rgb_keypad
  Main-Code.cpp
  usb_descriptors.c
  Settings.cpp
)

# Pull in pico libraries that we need
target_link_libraries(rgb_keypad pico_stdlib pico_rgb_keypad)

# create map/bin/hex file etc.
pico_add_extra_outputs(rgb_keypad)

# TINY USB STUFF
target_include_directories(rgb_keypad PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(rgb_keypad pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(rgb_keypad)

# add url via pico_set_program_url
# example_auto_set_url(rgb_keypad)

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(rgb_keypad pico_stdlib)

# enable usb output, disable uart output
#pico_enable_stdio_usb(rgb_keypad 1)
#pico_enable_stdio_uart(rgb_keypad 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(rgb_keypad)

# add url via pico_set_program_url
#example_auto_set_url(rgb_keypad)

