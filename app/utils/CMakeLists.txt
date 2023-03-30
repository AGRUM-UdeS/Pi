add_library(utils
        led.c
        utils.c
        )

target_link_libraries(utils
        pico_stdlib              # for core functionality
        )

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
