add_executable(measure
        measure.c
        )

target_include_directories(measure PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
        )

target_link_libraries(measure
        pico_stdlib              # for core functionality
        hardware_i2c
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(measure 1)
pico_enable_stdio_uart(measure 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(measure)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
