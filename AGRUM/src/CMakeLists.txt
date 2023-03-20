add_subdirectory(actuator)
add_subdirectory(I2C)
add_subdirectory(measure)
add_subdirectory(timing)
add_subdirectory(wifi)

add_executable(agrum
        main.c
        wifi/wifi.c
        wifi/mqtt.c
        wifi/ThingsBoard.c
        wifi/weather.c
        wifi/secret.c
        ../../pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
        ../../pico-sdk/lib/lwip/src/apps/http/http_client.c
        measure/measure.c
        actuator/actuator.c
        actuator/led/led.c
        actuator/motor/motor.c
        actuator/pump/pump.c
        actuator/valve/valve.c
        I2C/PI4IOE5V9554.c
        I2C/ADS7828.c
        I2C/SHT.c
        I2C/I2C_wrapper.c
        timing/ntp/ntp.c
        timing/rtc/rtc.c
        timing/timing.c
        )

target_include_directories(agrum PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_i2c
        hardware_pwm
        hardware_rtc
        pico_lwip
        pico_lwip_http
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
