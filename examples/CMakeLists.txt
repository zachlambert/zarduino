function(add_example name)
    add_executable(${name}.elf
        ${name}/main.c
    )
    target_link_libraries(${name}.elf zarduino)
endfunction()

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

add_executable(serial_write.elf
    serial_write/main.c
)
target_link_libraries(serial_write.elf zarduino)

add_example(serial_read)
