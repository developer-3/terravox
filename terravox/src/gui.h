//
//  gui.h
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#pragma once

#include <stdio.h>

#include <GLFW/glfw3.h>
#include "imgui.h"

#include "window.h"

namespace vpr {


    class GUI {
    public:
        GUI();
        ~GUI();

        void gui_init(GLFWwindow* window);
        void gui_start();
        void gui_end();

        void render();
        void setShowGui() { show = !show; }

    private:
        bool show = true;
    };
}
