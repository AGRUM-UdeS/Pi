add_library(sensors
        sensors.c
        )

target_link_libraries(sensors
        pico_stdlib              # for core functionality
        ADS7828
        PI4IOE5V9554A
        SHT
        )

target_include_directories(sensors PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )