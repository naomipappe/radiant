cmake_minimum_required(VERSION 3.22)
project(radiant)

set(CMAKE_CXX_STANDARD 20)

set(RADIANT_SOURCES src/main.cpp src/garbage/garbage_dump.cpp)

add_executable(radiant ${RADIANT_SOURCES})
add_subdirectory(external)

target_include_directories(radiant PRIVATE include)
target_link_libraries(radiant PRIVATE stb)