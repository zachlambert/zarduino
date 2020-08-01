add_executable(blink.elf
    blink/main.c
)
target_link_libraries(blink.elf zarduino)

add_executable(button.elf
    button/main.c
)
target_link_libraries(button.elf zarduino)

add_executable(analog_read.elf
    analog_read/main.c
)
target_link_libraries(analog_read.elf zarduino)
