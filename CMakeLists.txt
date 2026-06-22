cmake_minimum_required(VERSION 3.22)
project(radiant)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(RADIANT_SOURCES
        src/main.cpp
        src/garbage/garbage_dump.cpp
        src/core/camera.cpp
        src/core/material.cpp
        src/core/math.cpp
        src/importers/obj.cpp
        src/core/shape.cpp)

add_executable(radiant ${RADIANT_SOURCES} ${IMGUI_SRC})
add_subdirectory(external)

target_include_directories(radiant PRIVATE include)
target_link_libraries(radiant PRIVATE stb fmt::fmt rapidobj::rapidobj SDL3::SDL3 imgui)

find_package(OpenMP REQUIRED)
target_link_libraries(radiant PRIVATE OpenMP::OpenMP_CXX)