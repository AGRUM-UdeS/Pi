add_executable(pump
        pump.c
        )

target_include_directories(pump PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(pump
        pico_stdlib              # for core functionality
        hardware_pwm
        )
