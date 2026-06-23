set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME TechWaveAudio-MCM)

include(${CMAKE_SOURCE_DIR}/.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

# initialize the SDK based on PICO_SDK_PATH
include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

project(${PROJECT_NAME} CXX C ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_DEOPTIMIZED_DEBUG 1)

# initialize the Pico SDK
pico_sdk_init()

set(CMAKE_CXX_STANDARD 20)
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

file(GLOB_RECURSE MIDI_TO_CV_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE MIDI_TO_CV_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${MIDI_TO_CV_HEADERS} ${MIDI_TO_CV_SOURCES})

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c hardware_spi hardware_flash pico_multicore)

# create map/bin/hex/uf2 file in addition to ELF.
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})

