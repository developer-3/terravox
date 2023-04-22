//
//  window.h
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace tvox {

    class Window {
    public:
        Window(int w, int h, std::string name);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        void swapBuffers() { glfwSwapBuffers(window); }

        GLFWwindow* getWindow() { return window; }

    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow* window;
    };

}