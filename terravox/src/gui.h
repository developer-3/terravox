//
//  gui.h
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#pragma once

#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "imgui.h"

#include "window.h"

namespace tvox {


    class GUI {
    public:
        GUI();
        ~GUI();

        void gui_init(GLFWwindow* window);
        void gui_start();
        void gui_end();

        void render();
        void setShowGui() { show = !show; }

        void image(GLuint texture_id);

    private:
        bool show = true;
    };
}
