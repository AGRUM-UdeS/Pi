cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include_directories(lib/pico-sdk/lib/lwip/contrib/ports/freertos/include)
include_directories(lib/pico-sdk/lib/lwip/src/include)
include_directories(lib/FreeRTOS/FreeRTOS-SMP-Demos/FreeRTOS/Source/include)
include_directories(lib/FreeRTOS/FreeRTOS-SMP-Demos/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/include)

include(pico_sdk_import.cmake)

set(FREERTOS_KERNEL_PATH ../lib/FreeRTOS/FreeRTOS-SMP-Demos/FreeRTOS/Source)
include(lib/FreeRTOS/FreeRTOS-SMP-Demos/FreeRTOS/Demo/CORTEX_M0+_RP2040/Standard/FreeRTOS_Kernel_import.cmake)

set(LWIP_DIR lib/pico-sdk/lib/lwip)
include(lib/pico-sdk/external/pico_sdk_import.cmake)

project(AGRUM C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(agrum
        app/main.c
        )

# Add HAL, driver and lib directories
include_directories(app)
add_subdirectory(app)
add_subdirectory(hal)
add_subdirectory(driver)


add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(agrum 1)
pico_enable_stdio_uart(agrum 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(agrum)

# Disable useless waringn
target_link_options(agrum PRIVATE "LINKER:--no-warn-rwx-segments")


