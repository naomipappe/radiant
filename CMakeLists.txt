cmake_minimum_required(VERSION 3.22)
project(radiant)

set(CMAKE_CXX_STANDARD 20)

add_executable(radiant src/main.cpp src/sphere.cpp src/hittable_list.cpp)

target_include_directories(radiant PUBLIC include)
