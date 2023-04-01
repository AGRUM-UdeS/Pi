add_library(utils
        led.c
        utils.c
        test.h
        )

target_link_libraries(utils
        pico_stdlib              # for core functionality
        wrap_I2C
        )

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
