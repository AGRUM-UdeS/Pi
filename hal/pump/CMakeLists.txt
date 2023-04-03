add_library(pump
        pump.c
        )

target_link_libraries(pump
        pico_stdlib              # for core functionality
        PI4IOE5V9554A
        )

target_include_directories(pump PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
