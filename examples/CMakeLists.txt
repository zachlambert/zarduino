function(add_example name)
    add_executable(${name}.elf
        ${name}/main.c
    )
    target_link_libraries(${name}.elf zarduino)
    target_include_directories(${name}.elf
        PRIVATE /usr/lib/avr/include
    )
endfunction()

add_example(gpio_write)
add_example(gpio_read)
add_example(analog_read)
add_example(analog_read_interrupt)
add_example(serial_write)
add_example(serial_read)
add_example(interrupt_pin)
add_example(interrupt_external)
add_example(timer_pwm)
add_example(timer_timer)
add_example(timer_accurate)
add_example(timer_pulse)
add_example(spi_read_byte)
add_example(spi_read_bytes)
add_example(i2c)

add_example(module_mpu6050)
add_example(module_oled)
add_example(module_radio_rx)
add_example(module_radio_tx)
