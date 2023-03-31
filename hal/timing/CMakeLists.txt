add_library(timing
        timing.c
        )

target_link_libraries(timing
        hardware_rtc
        wrap_RTC
        wrap_NTP
        )

target_include_directories(timing PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

