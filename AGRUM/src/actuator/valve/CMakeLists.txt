add_executable(valve
        valve.c
        )

target_include_directories(valve PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(valve
        pico_stdlib              # for core functionality
        hardware_i2c
        hardware_pwm
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(valve 1)
pico_enable_stdio_uart(valve 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(valve)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
