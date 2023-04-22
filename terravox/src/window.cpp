//
//  window.cpp
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#include "window.h"

namespace tvox {

    Window::Window(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
    {
        initWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::initWindow()
    {
        if (!glfwInit())
            return;

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}
