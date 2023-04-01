add_library(wrap_MQTT
        wrap_MQTT.c
        ../../lib/pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
        )

target_link_libraries(wrap_MQTT
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        driver
        )

target_include_directories(wrap_MQTT PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

