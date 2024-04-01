# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
#set(PICO_SDK_PATH "/usr/share/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(nwt-sensor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)
add_subdirectory(lib/lwgps/lwgps)

add_executable(nwt-sensor
        main.c
        hw_config.c
        gps.c
        hcsr04.c
        sd.c
        data.c
        error.c
        control_panel.c
        led.c
        battery.c
)

pico_generate_pio_header(nwt-sensor ${CMAKE_CURRENT_LIST_DIR}/hcsr04.pio)
pico_generate_pio_header(nwt-sensor ${CMAKE_CURRENT_LIST_DIR}/led.pio)

pico_set_program_name(nwt-sensor "nwt-sensor")
pico_set_program_version(nwt-sensor "0.1")

pico_enable_stdio_uart(nwt-sensor 0)
pico_enable_stdio_usb(nwt-sensor 1)

# Add the standard library to the build
target_link_libraries(nwt-sensor
        pico_stdlib
        pico_multicore
        hardware_uart
        hardware_pio
        hardware_rtc
        hardware_adc
        FatFs_SPI
        lwgps
)

# Add the standard include files to the build
target_include_directories(nwt-sensor PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(nwt-sensor)

