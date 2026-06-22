add_subdirectory(stb EXCLUDE_FROM_ALL)

add_subdirectory(fmt EXCLUDE_FROM_ALL)

add_subdirectory(rapidobj EXCLUDE_FROM_ALL)

add_subdirectory(SDL-release-3.4.2 EXCLUDE_FROM_ALL)

# external/CMakeLists.txt
add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_sdl3.cpp
        imgui/backends/imgui_impl_sdlrenderer3.cpp)
target_include_directories(imgui PUBLIC imgui)
target_link_libraries(imgui PRIVATE SDL3::SDL3)