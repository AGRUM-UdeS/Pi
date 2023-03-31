add_library(weather
        weather.c
        )

target_link_libraries(weather
        pico_stdlib              # for core functionality
        wrap_WIFI
        )

target_include_directories(weather PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

