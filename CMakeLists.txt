cmake_minimum_required(VERSION 3.31)
project(vision_sensor_simulation)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(vision_sensor_simulation main.cpp)
