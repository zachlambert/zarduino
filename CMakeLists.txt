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
    src/core/pins.c
)
target_include_directories(zarduino
    PUBLIC include
)

add_subdirectory(examples)
