add_subdirectory(actuator)
add_subdirectory(I2C)
add_subdirectory(measure)
add_subdirectory(timing)
add_subdirectory(wifi)

add_executable(agrum
        main.c
        )

target_include_directories(agrum PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/src/measure
        ${CMAKE_CURRENT_LIST_DIR}/src/actuator
        ${CMAKE_CURRENT_LIST_DIR}/src/I2C
        ${CMAKE_CURRENT_LIST_DIR}/src/timing
        ${CMAKE_CURRENT_LIST_DIR}/src/wifi
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        pico_cyw43_arch_lwip_threadsafe_background
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# add url via pico_set_program_url
# example_auto_set_url(picow_blink)
