#pragma once

#include <functional>
#include <stdint.h>
#include <vector>
#include <mutex>

struct GLFWwindow;

namespace nt
{
    class IRenderer;
    typedef std::function<void(const nt::IRenderer *)> ImGuiRenderWindow;

    class IRendererLayer
    {
      public:
        virtual void Update() = 0;
        virtual ~IRendererLayer(){};
    };

    class IRenderer
    {
      protected:
        IRenderer(GLFWwindow *wnd);

        void RenderImGui();
        void RenderLayers();

      public:
        void AddWindow(const ImGuiRenderWindow &func);
        template <class TRenderLayer, typename... Args>
        void AddRenderLayer(Args &&...args);

        virtual ~IRenderer();
        virtual const char *GetRendererDescription() const = 0;
        virtual void Draw();

        virtual void InitRenderData(const uint32_t *const data);
        virtual void SetRenderData(const uint32_t *const data);

      private:
        std::vector<ImGuiRenderWindow> imGuiWindows;
        std::vector<IRendererLayer *> renderLayers;

        uint32_t width;
        uint32_t height;
      protected:
        std::mutex dataAccess;
        GLFWwindow *wnd;
        uint32_t* data;
    };

    template <class TRenderLayer, typename... Args>
    inline void IRenderer::AddRenderLayer(Args &&...args)
    {
        renderLayers.emplace_back(new TRenderLayer(args...));
    }
} // namespace nt