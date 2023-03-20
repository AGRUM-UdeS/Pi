add_executable(timing
        ntp/ntp.c
        rtc/rtc.c
        timing.c
        )

target_include_directories(timing PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src/timing
        )

target_link_libraries(timing
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_rtc
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(timing 1)
pico_enable_stdio_uart(timing 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(timing)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
