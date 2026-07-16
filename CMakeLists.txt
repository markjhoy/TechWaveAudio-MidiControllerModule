#
# Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
#
# SPDX-License-Identifier: BSD-3-Clause
#
#

set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME TechWaveAudio-MCM)

include(${CMAKE_SOURCE_DIR}/.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

# use our own TinyUSB submodule instead of the one under pico-sdk
set(PICO_TINYUSB_PATH ${CMAKE_SOURCE_DIR}/tinyusb)

# initialize the SDK based on PICO_SDK_PATH
include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

project(${PROJECT_NAME} CXX C ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_FLASH_ASSUME_CORE1_SAFE 1)

IF (DEBUG_BUILD EQUAL 1)
    set(PICO_DEOPTIMIZED_DEBUG 1)
ENDIF(DEBUG_BUILD EQUAL 1)

# initialize the Pico SDK
pico_sdk_init()

# add TinyUSB support for the rp2040 family
set(FAMILY rp2040)
include(${PICO_TINYUSB_PATH}/hw/bsp/family_support.cmake)

set(CMAKE_CXX_STANDARD 20)
IF (DEBUG_BUILD EQUAL 1)
    IF(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
    ENDIF(CMAKE_COMPILER_IS_GNUCC)
ENDIF(DEBUG_BUILD EQUAL 1)

file(GLOB_RECURSE MIDI_TO_CV_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE MIDI_TO_CV_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
add_compile_definitions(CFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/src/tusb_config.h")

add_executable(${PROJECT_NAME} ${MIDI_TO_CV_HEADERS} ${MIDI_TO_CV_SOURCES} src/usb_descriptors.c)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c hardware_spi hardware_flash pico_multicore tinyusb_device tinyusb_board)

pico_add_extra_outputs(${PROJECT_NAME})

