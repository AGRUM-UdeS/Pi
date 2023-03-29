add_executable(agrum
        main.c
        )

target_include_directories(agrum PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)
