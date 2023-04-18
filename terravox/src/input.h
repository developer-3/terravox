//
//  input.hpp
//  ecs_game
//
//  Created by Adam Anderson on 4/7/23.
//

#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <stdio.h>

namespace vpr {
    static constexpr int WIDTH = 1600;
    static constexpr int HEIGHT = 1240;

    class Input {
    public:

        Input();

        void PerFrame();
        
        void ProcessInput(GLFWwindow* window);

        glm::mat4 View() { return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); }
        glm::mat4 Perspective() { return glm::perspective(glm::radians(fov), (float)vpr::WIDTH / (float)vpr::HEIGHT, 0.1f, 100.0f); }


        void CursorPosition(double xpos, double ypos);
        void Scroll(double xoffset, double yoffset);

        // attributes
        // camera
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw = -90.0f;
        float pitch = 0.0f;
        float fov = 45.0f;

        // mouse state
        bool firstMouse = true;
        float lastX = vpr::WIDTH / 2.0f;
        float lastY = vpr::HEIGHT / 2.0f;

        // timing
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
    };

}
