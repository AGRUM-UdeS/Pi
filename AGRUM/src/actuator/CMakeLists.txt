add_subdirectory(led)
add_subdirectory(motor)
add_subdirectory(pump)
add_subdirectory(valve)

add_executable(actuator
        actuator.c
        led/led.c
        motor/motor.c
        pump/pump.c
        valve/valve.c
        )

target_include_directories(actuator PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/motor
        ${CMAKE_CURRENT_LIST_DIR}/led
        ${CMAKE_CURRENT_LIST_DIR}/pump
        ${CMAKE_CURRENT_LIST_DIR}/valve
        )

target_link_libraries(actuator
        pico_stdlib              # for core functionality
        hardware_i2c
        hardware_pwm
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(actuator 1)
pico_enable_stdio_uart(actuator 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(actuator)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
