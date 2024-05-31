#include "renderer.h"

nt::IRenderer::IRenderer(GLFWwindow *wnd) : wnd(wnd) {}

void nt::IRenderer::RenderImGui()
{
    for (const ImGuiRenderWindow render : imGuiWindows)
    {
        render(this);
    }
}

void nt::IRenderer::RenderLayers()
{
    for (nt::IRendererLayer *layer : renderLayers)
    {
        layer->Update();
    }
}

void nt::IRenderer::AddWindow(const ImGuiRenderWindow &func)
{
    imGuiWindows.emplace_back(func);
}

nt::IRenderer::~IRenderer()
{
    for (nt::IRendererLayer *layer : renderLayers)
    {
        delete layer;
    }
}

void nt::IRenderer::Draw() {}

void nt::IRenderer::InitRenderData(const uint32_t *const data) {}

void nt::IRenderer::SetRenderData(const uint32_t *const data) { (void)data; }
