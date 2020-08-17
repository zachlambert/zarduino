cmake_minimum_required(VERSION 3.0)
project(
    "Zarduino"
    VERSION 0.1.0
    DESCRIPTION "A library for controlling Arduino boards and AVR microcontrollers without using the Arduino libraries."
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(tools/target_cmake_scripts/arduino_uno.cmake)

add_library(zarduino STATIC
    src/comms/uart.c
    src/comms/spi.c
    src/comms/i2c.c
    src/core/adc.c
    src/core/interrupt.c
    src/core/pin_data.c
    src/core/regs.c
    src/module/mpu6050.c
    src/module/oled.c
    src/module/radio.c
    src/timing/timing.c
)
target_include_directories(zarduino
    PUBLIC include
    PRIVATE src /usr/lib/avr/include
)
# avr-gcc knows where the avr header files are, but putting
# it here allows the intellisense to know they exist

add_subdirectory(examples)

install(TARGETS zarduino
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include/zarduino PATTERN include/)
