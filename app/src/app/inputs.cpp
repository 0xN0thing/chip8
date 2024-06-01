#include "app.h"

#include <GLFW/glfw3.h>
#include <nt/chip8.h>

void nt::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mods)
{
    nt::App *app =
        reinterpret_cast<nt::App *>(glfwGetWindowUserPointer(window));
    app->InputHandler(key, scancode, action, mods);
}

void nt::App::SetupInputs(GLFWwindow *wnd)
{
    glfwSetWindowUserPointer(wnd, this);
    glfwSetKeyCallback(wnd, keyCallback);
}

void nt::App::InputHandler(int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_X:
    {
        vm->KeyPad()[0] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_1:
    {
        vm->KeyPad()[1] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_2:
    {
        vm->KeyPad()[2] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_3:
    {
        vm->KeyPad()[3] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_Q:
    {
        vm->KeyPad()[4] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_W:
    {
        vm->KeyPad()[5] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_E:
    {
        vm->KeyPad()[6] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_A:
    {
        vm->KeyPad()[7] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_S:
    {
        vm->KeyPad()[8] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_D:
    {
        vm->KeyPad()[9] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_Z:
    {
        vm->KeyPad()[0xA] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_C:
    {
        vm->KeyPad()[0xB] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_4:
    {
        vm->KeyPad()[0xC] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_R:
    {
        vm->KeyPad()[0xD] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_F:
    {
        vm->KeyPad()[0xE] = action == GLFW_PRESS;
        break;
    }
    case GLFW_KEY_V:
    {
        vm->KeyPad()[0xF] = action == GLFW_PRESS;
        break;
    }
    }
}