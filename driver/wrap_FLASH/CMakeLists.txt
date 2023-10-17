add_library(wrap_FLASH
        wrap_FLASH.c
        )

target_link_libraries(wrap_FLASH
        pico_stdlib              # for core functionality
        hardware_flash
        hardware_sync
        )

target_include_directories(wrap_FLASH PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

