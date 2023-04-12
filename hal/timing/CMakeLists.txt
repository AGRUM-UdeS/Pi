add_library(timing
        timing.c
        )

target_link_libraries(timing
        interface
        hardware_rtc
        wrap_RTC
        wrap_NTP
        wrap_WATCHDOG
        )

target_include_directories(timing PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

