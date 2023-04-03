add_library(PI4IOE5V9554A
        PI4IOE5V9554A.c
        )

target_link_libraries(PI4IOE5V9554A
        pico_stdlib              # for core functionality
        wrap_I2C
        )

target_include_directories(PI4IOE5V9554A PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

