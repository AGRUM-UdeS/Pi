add_library(irrigation
        irrigation.c
        )

target_link_libraries(irrigation
        pico_stdlib              # for core functionality
        utils
        pump
        valve
        sensors
        interface
        )

include_directories("${CMAKE_SOURCE_DIR}/app/utils")

target_include_directories(irrigation PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
