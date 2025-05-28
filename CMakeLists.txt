cmake_minimum_required(VERSION 3.22)

set(PICO_PLATFORM "rp2350-arm-s" CACHE STRING "Targeting pico 2350 specifically the arm core")

set(SOURCE_DIR "src/")

set(PROJ_NAME sbmproj)
set(TARG_NAME sbm)

if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "PICO_SDK_PATH env not set")
endif()

# initialize the SDK based on PICO_SDK_PATH
# note: this happens before the project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJ_NAME})

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# adding library
add_library(${TARG_NAME} STATIC
    ${SOURCE_DIR}SH1106_Interactions.c
)

target_include_directories(${TARG_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${TARG_NAME} PUBLIC
    #pico_stdlib
    pico_time
    hardware_timer
    hardware_gpio
    hardware_spi    
)

# install $TARG_NAME static lib and header files for integration by other programs
install(TARGETS ${TARG_NAME} DESTINATION /usr/local/lib)
install(DIRECTORY include/ DESTINATION /usr/local/include/)
