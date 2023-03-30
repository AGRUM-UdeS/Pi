add_library(wrap_WIFI
        wrap_WIFI.c
        )

target_link_libraries(wrap_WIFI
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        secret
        driver
        )

target_include_directories(wrap_WIFI PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

