add_library(interface
        interface.c
        )

target_link_libraries(interface
        pico_stdlib              # for core functionality
        wrap_WIFI
        ThingsBoard
        timing
        )

target_include_directories(interface PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

