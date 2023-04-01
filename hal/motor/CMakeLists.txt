add_library(motor
        motor.c
        )

target_link_libraries(motor
        pico_stdlib              # for core functionality
        PI4IOE5V9554A
        wrap_PWM
        )

target_include_directories(motor PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
