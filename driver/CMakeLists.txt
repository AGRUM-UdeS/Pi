# CMakeLists.txt for the driver directory

# Add all driver subdirectories
# add_subdirectory(ADS7828)
# add_subdirectory(OpenMeteo)
# add_subdirectory(PI4IOE5V9554A)
# add_subdirectory(secret)
# add_subdirectory(SHT)
# add_subdirectory(ThingsBoard)

# add_subdirectory(wrap_MQTT)
# add_subdirectory(wrap_GPIO)
# add_subdirectory(wrap_HTTP)
# add_subdirectory(wrap_I2C)
# add_subdirectory(wrap_NTP)
# add_subdirectory(wrap_RTC)
# add_subdirectory(wrap_WATCHDOG)
# add_subdirectory(wrap_WIFI)


# Add all the source files in the driver directory to the hal library
add_library(driver)

# Specify any libraries or dependencies that the driver library requires
target_link_libraries(driver
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip
    pico_lwip_http

    hardware_i2c
    hardware_pwm
    hardware_rtc
)

# Include the current directory in the include path for the hal library
# target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
