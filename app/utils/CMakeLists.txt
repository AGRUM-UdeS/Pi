add_library(utils
        led.c
        utils.c
        test.c
        )

target_link_libraries(utils
        pico_stdlib              # for core functionality
        irrigation
        pv_management
        energy_management
        sensors
        interface
        wrap_WATCHDOG
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        )

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
