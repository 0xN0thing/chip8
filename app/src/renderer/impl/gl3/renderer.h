#pragma once

#include "renderer/renderer.h"
#include <string>

namespace nt
{
    namespace GL3
    {
        class RendererImpl : public IRenderer
        {

          public:
            RendererImpl(GLFWwindow *wnd);
            ~RendererImpl();

            const char *GetRendererDescription() const override;

            void InitRenderData(const uint32_t *const data) override;
            void SetRenderData(const uint32_t *const data) override;

            void Draw() override;

          private:
            int displayWidth;
            int displayHeight;

            int supportedGLVersion;

            std::string desc;

          private:
            void RenderObjectsInitialization();
            void FreeRenderObjects();
            void DrawRenderData();

            unsigned int VBO;
            unsigned int VAO;
            unsigned int EBO;
            unsigned int texture;
            unsigned int shader;

          private:
            void ImGuiBeginFrame();
            void ImGuiShutdown();
        };

    } // namespace GL3
} // namespace nt