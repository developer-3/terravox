#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "window.h"
#include "gui.h"
#include "input.h"
#include "graphics/Mesh.h"
#include "graphics/Texture.h"
#include "graphics/CubeTexture.h"
#include "graphics/Voxel.h"

namespace tvox {

    class Application {
    public:

        void run();

        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

        void RenderGL();

        // Voxel Gen
        template <typename T>
        T Clamp(const T& value, const T& minValue, const T& maxValue);

        std::vector<glm::vec3> generate_voxel(int horz_axis_res, float vert_axis_ratio, float scale, int depth, float param_1 = 3, float param_2 = -2, float param_3 = -2);
        float hilly_terrain(float x, float y, float scale = 1, float param_1 = 3, float param_2 = -2, float param_3 = -2);

        // meshes
        CubeTexture skybox_tex;
        CMesh skybox;

        Texture texture_atlas;

        Voxel vox{ 0 };

        std::vector<glm::vec3> cubePositions;

        int genVoxelNumber = 200;

    private:
        Window window{ tvox::WIDTH, tvox::HEIGHT, "terravox" };
        static tvox::GUI gui;
        static Input input;

        GLuint m_program;

    };
}