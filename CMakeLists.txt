# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(garage)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")
include_directories(include)

file(GLOB SOURCES "source/*.cpp")

add_executable(garage ${SOURCES})

find_library(wiringPi_LIB wiringPi)
target_link_libraries(garage ${wiringPi_LIB})

find_library(mosquitto_LIB mosquittopp)
target_link_libraries(garage ${mosquitto_LIB})
