add_library(ThingsBoard
        ThingsBoard.c
        )

target_link_libraries(ThingsBoard
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_sys_freertos

        wrap_MQTT
        wrap_RTC
        wrap_WATCHDOG
        )

target_include_directories(ThingsBoard PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

