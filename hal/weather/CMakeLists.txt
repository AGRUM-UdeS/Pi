add_library(weather
        weather.c
        ../../lib/pico-sdk/lib/lwip/src/apps/http/http_client.c
        )

target_link_libraries(weather
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        pico_lwip_http
        wrap_WIFI
        )

target_include_directories(weather PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../..
        )

