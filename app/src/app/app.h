#pragma once

#include <chrono>
#include <string>
#include <stdint.h>

struct GLFWwindow;
#include <thread>

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


    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

        friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

      private:
        IRenderer *renderer;

        GLFWwindow *wnd;

      private:
        void SetupInputs(GLFWwindow* wnd);
 
        void InputHandler(int key, int scancode, int action, int mods);
        
        float dt;

        std::chrono::time_point<std::chrono::high_resolution_clock>
            lastCycleTime;
        chip8::IVirtualMachine *vm;

        std::string lastOpenedFirmware;

        bool work;
        bool showChip8Window;
        bool showRendererWindow;

        int cycleDelay;
    };

    template <class TRendererImpl> inline void App::InstallRenderer()
    {
        renderer = new TRendererImpl(wnd);
        OnNewRendererInstalled();
    }

} // namespace nt