cmake_minimum_required(VERSION 2.6)
project(zarduino)

# set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Settings for arduino UNO

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(COPT "-Os")
SET(CMCU "-mmcu=atmega328")
SET(CDEFS "-DF_CPU=16000000")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

SET(CFLAGS "${CMCU} ${CDEFS} ${COPT} ${CSTANDARD}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${COPT}")

SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

add_library(zarduino
    src/core/pins.c
)
target_include_directories(zarduino
    PUBLIC include
)

add_subdirectory(examples)
