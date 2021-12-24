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
