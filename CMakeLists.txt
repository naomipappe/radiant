cmake_minimum_required(VERSION 3.22)
project(RayTracing1Weekend)

set(CMAKE_CXX_STANDARD 20)

add_executable(RayTracing1Weekend src/main.cpp src/sphere.cpp src/hittable_list.cpp)

target_include_directories(RayTracing1Weekend PUBLIC include)
