add_library(
    stb 
    STATIC
    src/stb_image_write.cpp src/stb_image.cpp
)

target_include_directories(
    stb
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)