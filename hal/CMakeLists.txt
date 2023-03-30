# CMakeLists.txt for the hal directory

# Add all hal subdirectories
add_subdirectory(interface)
# add_subdirectory(motor)
add_subdirectory(pump)
# add_subdirectory(sensor)
add_subdirectory(time)
# add_subdirectory(valve)

# Add all the source files in the hal directory to the hal library
add_library(hal)

# Specify any libraries or dependencies that the hal library requires
target_link_libraries(hal
        driver
        interface
)

# Include the current directory in the include path for the hal library
target_include_directories(hal PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
