add_executable(I2C_wrapper
        I2C_wrapper.c
        PI4IOE5V9554.c
        ADS7828.c
        SHT.c
        )

target_include_directories(I2C_wrapper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(I2C_wrapper
        pico_stdlib              # for core functionality
        hardware_i2c
        )
