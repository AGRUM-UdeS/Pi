add_library(energy_management
        energy_management.c
        )

target_link_libraries(energy_management
        pico_stdlib              # for core functionality
        )

target_include_directories(energy_management PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )