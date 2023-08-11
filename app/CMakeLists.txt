add_subdirectory(energy_management)
add_subdirectory(pv_management)
add_subdirectory(irrigation)
add_subdirectory(utils)

target_include_directories(agrum PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(agrum
        pico_stdlib              # for core functionality
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        irrigation
        pv_management
        energy_management
        interface
        hal
        utils
        weather
        )
