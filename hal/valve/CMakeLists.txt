add_library(valve
        valve.c
        )

target_link_libraries(valve
        pico_stdlib              # for core functionality
        PI4IOE5V9554A
        hardware_pwm
        )

target_include_directories(valve PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )