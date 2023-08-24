add_library(pv_management
        pv_management.c
        )

target_link_libraries(pv_management
        pico_stdlib              # for core functionality
        utils
        motor
        )

target_include_directories(pv_management PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
