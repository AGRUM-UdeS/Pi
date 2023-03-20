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

# enable usb output, disable uart output
pico_enable_stdio_usb(I2C_wrapper 1)
pico_enable_stdio_uart(I2C_wrapper 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(I2C_wrapper)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
