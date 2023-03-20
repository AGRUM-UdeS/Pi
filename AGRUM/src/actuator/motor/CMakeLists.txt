add_executable(motor
        motor.c
        )

target_link_libraries(motor
        pico_stdlib              # for core functionality
        hardware_pwm
        )

