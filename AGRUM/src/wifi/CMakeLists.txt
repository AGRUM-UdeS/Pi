add_executable(wifi
        wifi.c
        mqtt.c
        ThingsBoard.c
        weather.c
        secret.c
        ../../../pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
        ../../../pico-sdk/lib/lwip/src/apps/http/http_client.c
        )

target_include_directories(wifi PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
        )

target_link_libraries(wifi
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        pico_lwip_http
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(wifi 1)
pico_enable_stdio_uart(wifi 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(wifi)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
