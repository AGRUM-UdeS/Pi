add_executable(led
        led.c
        )

target_include_directories(led PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(led
        pico_stdlib              # for core functionality
        hardware_pwm
        )

