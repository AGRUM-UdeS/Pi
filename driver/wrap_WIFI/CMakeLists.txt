add_library(wifi
        wifi.c
        )

target_link_libraries(wifi
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        secret
        driver
        )

target_include_directories(interface PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

