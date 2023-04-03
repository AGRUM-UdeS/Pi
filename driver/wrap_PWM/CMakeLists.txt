add_library(wrap_PWM
        wrap_PWM.c
        )

target_link_libraries(wrap_PWM
        pico_stdlib              # for core functionality
        hardware_pwm
        )

target_include_directories(wrap_PWM PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

