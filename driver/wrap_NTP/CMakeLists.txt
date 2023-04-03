add_library(wrap_NTP
        wrap_NTP.c
        )

target_link_libraries(wrap_NTP
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        driver
        wrap_RTC
        )

target_include_directories(wrap_NTP PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

