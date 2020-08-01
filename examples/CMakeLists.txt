add_executable(blink.elf
    blink/main.c
)
target_link_libraries(blink.elf zarduino)

add_executable(button.elf
    button/main.c
)
target_link_libraries(button.elf zarduino)
