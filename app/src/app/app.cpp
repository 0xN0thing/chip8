#include "app.h"
#include "renderer/renderer.h"

#include <glad/gl.h>
// #include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <nt/chip8_std_impl.h>

#include <iostream>
#include <stdexcept>

#include <imgui.h>

nt::App::App(const char *title, uint32_t width, uint32_t height, int cycleDelay)
    : wnd(glfwCreateWindow(width, height, title, NULL, NULL)),
      vm(new nt::chip8::VMImpl), renderer(NULL),
      lastCycleTime(std::chrono::high_resolution_clock::now()),
      cycleDelay(cycleDelay)

{
    if (wnd == NULL)
        throw std::runtime_error("failed to create window!");
}

bool nt::App::IsActive() { return !glfwWindowShouldClose(wnd); }

void nt::App::Update()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
                   currentTime - lastCycleTime)
                   .count();

    glfwPollEvents();

    if (dt > cycleDelay)
    {
        lastCycleTime = currentTime;
        vm->Update();
        renderer->SetRenderData(vm->GetVideoBuffer());
    }

    if (renderer != NULL)
        renderer->Draw();
}

void nt::App::InstallVirtualMachine(chip8::IVirtualMachine *vm)
{
    this->vm = vm;

    OnNewVirtualMachineInstalled();
}

void nt::App::OnNewRendererInstalled()
{
    renderer->AddWindow(
        [this](const IRenderer *renderer) -> void
        {
            ImGui::Begin(renderer->GetRendererDescription());
            ImGui::Text("renderer:");
            ImGui::Text(renderer->GetRendererDescription());
            ImGui::SliderInt("CycleDelay", &cycleDelay, 5, 100000);
            if (ImGui::Button("restart"))
            {
                vm->ResetPC();
            }
            ImGui::End();
        });
}

void nt::App::OnNewVirtualMachineInstalled()
{
    renderer->InitRenderData(vm->GetVideoBuffer());
}

nt::App::~App() { glfwDestroyWindow(wnd); }
