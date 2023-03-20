add_executable(pump
        pump.c
        )

target_include_directories(pump PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(pump
        pico_stdlib              # for core functionality
        hardware_pwm
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(pump 1)
pico_enable_stdio_uart(pump 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pump)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
