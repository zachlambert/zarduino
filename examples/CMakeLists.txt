function(add_example name)
    add_executable(${name}.elf
        ${name}/main.c
    )
    target_link_libraries(${name}.elf zarduino)
endfunction()

add_example(blink)
add_example(button)
add_example(analog_read)
add_example(analog_read_interrupt)
add_example(serial_write)
add_example(serial_read)
add_example(interrupt_pin)
add_example(interrupt_external)
add_example(timer_pwm)
add_example(timer_timer)
add_example(timer_accurate)
