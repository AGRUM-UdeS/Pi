add_library(irrigation
        irrigation.c
        )

target_link_libraries(irrigation
        pico_stdlib              # for core functionality
        pump
        valve
        sensors
        interface
        )

target_include_directories(irrigation PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
