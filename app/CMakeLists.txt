cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("chip8")

include("cmake/CPM.cmake")

add_executable(chip8_exec "src/main.cpp" 
"src/app/app.h" "src/app/app.cpp"
"src/app/glfw.cpp"
"src/renderer/renderer.cpp" "src/renderer/renderer.h"
"src/renderer/impl/gl3/renderer.h" "src/renderer/impl/gl3/renderer.cpp"
"src/renderer/impl/gl3/shared/shader.h" "src/renderer/impl/gl3/shared/shader.cpp"
)

CPMAddPackage("gh:glfw/glfw#b35641f4a3c62aa86a0b3c983d163bc0fe36026d")

add_subdirectory("external")

target_link_libraries(chip8_exec PRIVATE chip8 glad glfw imgui)

target_include_directories(chip8_exec PRIVATE "src/")