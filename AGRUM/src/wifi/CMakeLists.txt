add_executable(wifi
        wifi.c
        mqtt.c
        ThingsBoard.c
        weather.c
        secret.c
        ../../../pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
        ../../../pico-sdk/lib/lwip/src/apps/http/http_client.c
        )

target_include_directories(wifi PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/wifi
        )

target_link_libraries(wifi
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        pico_lwip_http
        )
