#include "renderer/impl/gl3/renderer.h"
#include "renderer/impl/gl3/shared/shader.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar *message,
                                     const void *userParam)
{
    printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
           severity, message);
}

void nt::GL3::RendererImpl::RenderObjectsInitialization()
{
    float vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f  // top left
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   TexCoord = aTexCoord;\n"
        "}\0";

    // Fragment Shader
    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture1;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(texture1, TexCoord);\n"
        "}\n\0";

    shader = nt::GL3::CompileAndLinkShaders(vertexShaderSource,
                                            fragmentShaderSource);
}

void nt::GL3::RendererImpl::FreeRenderObjects()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader);
}

void nt::GL3::RendererImpl::DrawRenderData()
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void nt::GL3::RendererImpl::ImGuiBeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void nt::GL3::RendererImpl::ImGuiShutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

nt::GL3::RendererImpl::RendererImpl(GLFWwindow *wnd)
    : nt::IRenderer::IRenderer(wnd), supportedGLVersion(0),
      desc("default nt renderer: opengl 0.0 not initialized!"), VBO(0), VAO(0),
      EBO(0)
{
    data = NULL;
    glfwMakeContextCurrent(wnd);

    supportedGLVersion = gladLoadGL(glfwGetProcAddress);
    if (supportedGLVersion == 0)
        throw std::runtime_error("failed to initialize glad!");

    std::cout << "supported opengl version is "
              << GLAD_VERSION_MAJOR(supportedGLVersion) << "."
              << GLAD_VERSION_MINOR(supportedGLVersion) << std::endl;

    {
        std::stringstream ss;
        ss << "default nt renderer: opengl "
            << GLAD_VERSION_MAJOR(supportedGLVersion) << "."
            << GLAD_VERSION_MINOR(supportedGLVersion);
        
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);

        if (renderer && vendor) 
        {
           ss << "(" << glGetString(GL_RENDERER) << " " << glGetString(GL_VENDOR) << ")";
        }

        desc = ss.str();
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(wnd, true);
    const char *glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    RenderObjectsInitialization();
}

nt::GL3::RendererImpl::~RendererImpl()
{
    FreeRenderObjects();
    ImGuiShutdown();
}

const char *nt::GL3::RendererImpl::GetRendererDescription() const
{
    return desc.c_str();
}

void nt::GL3::RendererImpl::InitRenderData(const uint32_t *const data)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void nt::GL3::RendererImpl::SetRenderData(const uint32_t *const data)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGBA, GL_UNSIGNED_BYTE,
                    data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void nt::GL3::RendererImpl::Draw()
{
    ImGuiBeginFrame();

    RenderImGui();

    ImGui::Render();

    glfwGetFramebufferSize(wnd, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawRenderData();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(wnd);
}
