cmake_minimum_required(VERSION 3.22)
project(radiant)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(RADIANT_SOURCES 
src/main.cpp 
src/garbage/garbage_dump.cpp 
src/core/sphere.cpp
src/core/linear_aggregate.cpp
src/core/camera.cpp
src/core/material.cpp
src/core/math.cpp
src/importers/obj.cpp
src/core/shapes/triangle.cpp)

add_executable(radiant ${RADIANT_SOURCES})
add_subdirectory(external)

target_include_directories(radiant PRIVATE include)
target_link_libraries(radiant PRIVATE stb fmt::fmt rapidobj::rapidobj)