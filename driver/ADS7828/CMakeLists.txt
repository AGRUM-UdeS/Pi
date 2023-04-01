add_library(ADS7828
        ADS7828.c
        )

target_link_libraries(ADS7828
        pico_stdlib              # for core functionality
        wrap_I2C
        )

target_include_directories(ADS7828 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

