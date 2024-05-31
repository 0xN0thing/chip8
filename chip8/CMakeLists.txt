
option(ADD_STD_LIB_IMPL "build with standard c++ library" ON)

if(ADD_STD_LIB_IMPL)
set(STD_IMPL_SOURCES "src/std_impl.cpp" "inc/nt/chip8_std_impl.h")
endif(ADD_STD_LIB_IMPL)

add_library(chip8 "src/chip8.cpp" "inc/nt/chip8.h" ${STD_IMPL_SOURCES} "src/instructions.cpp")


if(ADD_DESKTOP_IMPL)
target_compile_definitions(chip8 PUBLIC NT_CHIP_STD)
endif(ADD_DESKTOP_IMPL)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chip8 PROPERTY CXX_STANDARD 14)
endif()

target_include_directories(chip8 PUBLIC "inc/")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
