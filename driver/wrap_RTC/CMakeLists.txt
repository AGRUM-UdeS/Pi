add_library(wrap_RTC
        wrap_RTC.c
        )

target_link_libraries(wrap_RTC
        hardware_rtc
        wrap_NTP
        )

target_include_directories(wrap_RTC PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

