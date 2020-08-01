cmake_minimum_required(VERSION 3.0)
project(
    "Zarduino"
    VERSION 0.1.0
    DESCRIPTION "A library for controlling Arduino boards and AVR microcontrollers without using the Arduino libraries."
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(tools/target_cmake_scripts/arduino_uno.cmake)

add_library(zarduino
    src/core/adc.c
    src/core/pins.c
    src/core/regs.c
    src/comms/uart.c
)
target_include_directories(zarduino
    PUBLIC include
    PRIVATE /usr/lib/avr/include
)
# avr-gcc knows where the avr header files are, but putting
# it here allows the intellisense to know they exist

add_subdirectory(examples)
