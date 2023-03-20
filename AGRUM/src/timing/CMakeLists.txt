add_executable(timing
        ntp/ntp.c
        rtc/rtc.c
        timing.c
        )

target_include_directories(timing PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src/timing
        )

target_link_libraries(timing
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_rtc
        )
