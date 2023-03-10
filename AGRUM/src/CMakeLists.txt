include_directories(measure)
include_directories(actuator)
include_directories(actuator/led)
include_directories(actuator/motor)
include_directories(actuator/pump)
include_directories(actuator/valve)
include_directories(thingsboard)

include_directories(I2C)
include_directories(timing)
include_directories(timing/ntp)
include_directories(timing/rtc)

add_executable(agrum
        main.c
        thingsboard/wifi.c
        thingsboard/mqtt.c
        thingsboard/ThingsBoard.c
        ../../pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
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
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${CMAKE_CURRENT_LIST_DIR}/src
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        hardware_rtc
        hardware_i2c
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
