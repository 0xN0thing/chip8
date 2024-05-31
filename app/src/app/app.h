#pragma once

#include <chrono>
#include <stdint.h>

struct GLFWwindow;

namespace nt
{
    class IRenderer;

    namespace chip8
    {
        class IVirtualMachine;
    }

    class GLFW
    {
      public:
        GLFW();
        ~GLFW();
    };

    class App
    {
      public:
        App(const char *title, uint32_t width, uint32_t height, int cycleDelay);
        ~App();

        bool IsActive();
        void Update();

        template <class TRendererImpl> void InstallRenderer();

        void InstallVirtualMachine(chip8::IVirtualMachine *vm);

      private:
        void OnNewRendererInstalled();
        void OnNewVirtualMachineInstalled();

      private:
        IRenderer *renderer;

        GLFWwindow *wnd;

      private:
        std::chrono::time_point<std::chrono::high_resolution_clock>
            lastCycleTime;
        chip8::IVirtualMachine *vm;

        int cycleDelay;
    };

    template <class TRendererImpl> inline void App::InstallRenderer()
    {
        renderer = new TRendererImpl(wnd);
        OnNewRendererInstalled();
    }

} // namespace nt