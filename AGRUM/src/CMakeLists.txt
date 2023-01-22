add_executable(agrum
        main.c
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
