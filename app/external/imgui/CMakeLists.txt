cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("chip8")

file(GLOB IMGUI_SRC
     "*.h"
     "*.cpp"
)

set(IMGUI_BACKEND "backend/imgui_impl_glfw.cpp" "backend/imgui_impl_glfw.h" "backend/imgui_impl_opengl3.cpp" "backend/imgui_impl_opengl3.h")

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_BACKEND})

target_include_directories(imgui PUBLIC "./" "./backend")

target_link_libraries(imgui PUBLIC glfw glad)