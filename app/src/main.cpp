#include "app/app.h"
#include "renderer/impl/gl3/renderer.h"
#include <nt/chip8_std_impl.h>

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        nt::chip8::VMImpl vm;

        nt::GLFW glfw{};

        nt::App app{"CHIP8", 1280, 640, 3};

        app.InstallRenderer<nt::GL3::RendererImpl>();
        app.InstallVirtualMachine(&vm);

        while (app.IsActive())
        {
            app.Update();
        }
    }
    catch (std::runtime_error error)
    {
        std::cout << error.what() << std::endl;
    }
    return 0;
}