add_executable(measure
        measure.c
        )

target_include_directories(measure PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src/measure
        )

target_link_libraries(measure
        pico_stdlib              # for core functionality
        hardware_i2c
        )
