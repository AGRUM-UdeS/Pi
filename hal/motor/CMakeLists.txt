add_library(motor
        motor.c
        )

target_link_libraries(motor
        pico_stdlib              # for core functionality
        hardware_pwm
        )

target_include_directories(motor PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
