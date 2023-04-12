add_library(wrap_WATCHDOG
        wrap_WATCHDOG.c
        )

target_link_libraries(wrap_WATCHDOG
        pico_stdlib              # for core functionality
        )

target_include_directories(wrap_WATCHDOG PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

