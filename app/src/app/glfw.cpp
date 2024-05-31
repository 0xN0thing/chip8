#include "app.h"

#include <iostream>
#include <stdexcept>

#include <GLFW/glfw3.h>

void glfwErrorHandler(int code, const char *desc)
{
    std::cout << "glfw error:" << desc << " code:" << code << std::endl;
}

nt::GLFW::GLFW()
{
    glfwSetErrorCallback(glfwErrorHandler);

    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("failed to initialize glfw!");
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

nt::GLFW::~GLFW() { glfwTerminate(); }
