add_executable(valve
        valve.c
        )

target_include_directories(valve PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(valve
        pico_stdlib              # for core functionality
        hardware_i2c
        hardware_pwm
        )
