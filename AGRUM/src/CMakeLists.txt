include_directories(measure)
include_directories(actuator)
include_directories(actuator/led)
include_directories(actuator/motor)
include_directories(actuator/pump)
include_directories(actuator/valve)

add_executable(agrum
        main.c
        wifi.c
        mqtt.c
        ThingsBoard.c
        ../../pico-sdk/lib/lwip/src/apps/mqtt/mqtt.c
        measure/measure.c
        actuator/actuator.c
        actuator/led/led.c
        actuator/motor/motor.c
        actuator/pump/pump.c
        actuator/valve/valve.c
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
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
