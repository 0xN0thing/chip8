#include "app.h"
#include "renderer/renderer.h"

#include <glad/gl.h>
// #include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <nt/chip8_std_impl.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include <imgui.h>


void writeVideoToFile(const uint32_t video[], int width, int height, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    outputFile << std::setw(8) << std::setfill('0');

    // Iterate through each pixel in the video array
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Calculate the index of the current pixel in the video array
            int index = y * width + x;

            // Write the hex value of the pixel to the file
            outputFile << std::hex << video[index] << " ";

            // Add newline after each row
            if (x == width - 1) {
                outputFile << std::endl;
            }
        }
    }

    outputFile.close();
    std::cout << "Video data has been written to " << filename << std::endl;
}

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
            ImGui::InputInt("CycleDelay", &cycleDelay, 1, 10);
            if (ImGui::Button("restart"))
            {
                vm->ResetPC();
            }
            if (ImGui::Button("save video to file"))
            {
                writeVideoToFile(vm->GetVideoBuffer(), 32, 64, "video.txt");
            }
            ImGui::End();
        });
}

void nt::App::OnNewVirtualMachineInstalled()
{
    memset(buffer, 0, 64 * 32);

    for (uint32_t i = 0; i < 64 * 32; i += 1)
    {
        if(i % 2 == 0)
            buffer[i] = 0xfffffff;
    }

    renderer->InitRenderData(buffer);
}

nt::App::~App() { glfwDestroyWindow(wnd); }
