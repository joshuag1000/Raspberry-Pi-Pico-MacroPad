add_executable(
  rgb_keypad
  Main.cpp
  RGBMacroLibrary.cpp
  usb_descriptors.c
)

# Pull in pico libraries that we need
target_include_directories(rgb_keypad PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(rgb_keypad pico_stdlib pico_rgb_keypad tinyusb_device tinyusb_board)


# create map/bin/hex file etc.
pico_add_extra_outputs(rgb_keypad)
