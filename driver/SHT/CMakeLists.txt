add_library(SHT
        SHT.c
        )

target_link_libraries(SHT
        pico_stdlib              # for core functionality
        wrap_I2C
        )

target_include_directories(SHT PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

