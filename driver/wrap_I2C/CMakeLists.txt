add_library(wrap_I2C
        wrap_I2C.c
        )

target_link_libraries(wrap_I2C
        pico_stdlib              # for core functionality
        hardware_i2c
        )

target_include_directories(wrap_I2C PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

