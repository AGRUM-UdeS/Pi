add_executable(led
        led.c
        )

target_include_directories(led PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(led
        pico_stdlib              # for core functionality
        hardware_pwm
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(led 1)
pico_enable_stdio_uart(led 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(led)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
