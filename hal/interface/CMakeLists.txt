add_library(interface
        interface.c
        )

target_link_libraries(interface
        pico_stdlib              # for core functionality
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        utils
        wrap_WIFI
        ThingsBoard
        timing
        sensors
        )

include_directories("${CMAKE_SOURCE_DIR}/app/utils")

target_include_directories(interface PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

