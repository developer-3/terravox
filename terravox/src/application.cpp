//
//  application.cpp
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#include "application.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include "Terrain.h"

namespace tvox {

    tvox::GUI Application::gui;
    tvox::Input Application::input;

    void Application::run() {

        glViewport(0, 0, WIDTH, HEIGHT);

        // Initialize glfw callbacks for key and mouse input
        glfwSetMouseButtonCallback(window.getWindow(), Application::mouse_button_callback);
        glfwSetCursorPosCallback(window.getWindow(), Application::cursor_position_callback);
        glfwSetScrollCallback(window.getWindow(), Application::scroll_callback);

        // Init gui after setting glfw callbacks
        gui.gui_init(window.getWindow());

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            return;
        }

        int major, minor;
        char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

        major = ver[0] - '0';
        if (major >= 3)
        {
            // for GL 3.x
            glGetIntegerv(GL_MAJOR_VERSION, &major); // major = 3
            glGetIntegerv(GL_MINOR_VERSION, &minor); // minor = 2
        }
        else
        {
            minor = ver[2] - '0';
        }

        if (major < 3 || (major == 3 && minor < 2)) {
            std::cout << "OpenGL 3.2 is not supported!" << std::endl;
            std::cout << "Using OpenGL version " << major << "." << minor << std::endl;
        }


        // Loading in textures and objs
        skybox_tex.LoadFiles("src/textures/right.jpg", "src/textures/left.jpg", "src/textures/top.jpg", "src/textures/bottom.jpg", "src/textures/front.jpg", "src/textures/back.jpg");
        skybox.CreateCube();
        skybox.m_program = LoadShaders("src/shaders/vertexsky.glsl", "src/shaders/fragmentSky.glsl");
        skybox.InitGL();

        texture_atlas.LoadFile("src/textures/block/RPGpack_sheet_2X.png");

        // Init voxels
        //cubePositions = generate_voxel(genVoxelNumber, 0.3f, 1.0f, 50);
        cubePositions = Terrain::GeneratePerlinTerrain(200, 0, 40);

        vox.CreateCube();
        vox.m_program = LoadShaders("src/shaders/vVoxelInstance.glsl", "src/shaders/fragment-basic.glsl");
        vox.InitGL(cubePositions);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        while (!window.shouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // per-frame time logic
            input.PerFrame();

            // Process input
            input.ProcessInput(window.getWindow());
            
            // render the scene
            RenderGL();

            // render gui
            gui.gui_start();
            ImGui::Begin("Voxels");
            bool changed = ImGui::SliderInt("Voxels", &genVoxelNumber, 30, 200);
            ImGui::Text("Instances: %d", cubePositions.size());

            ImGui::SliderFloat("Camera Speed", &input.speed, 0, 100);

            ImGui::End();
            gui.gui_end();

            // this is rly slow to do at runtime
            if (changed)
            {
                cubePositions = generate_voxel(genVoxelNumber, 0.5f, 1.0f, 3);
                vox.InitGL(cubePositions);
            }

            /* Swap front and back buffers */
            window.swapBuffers();
            glfwPollEvents();
        }
    }

    void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
    }

    void Application::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGuiIO& io = ImGui::GetIO();
        
        if (!io.WantCaptureMouse)
            input.CursorPosition(xpos, ypos);
    }

    void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        input.Scroll(xoffset, yoffset);
    }

    void Application::RenderGL()
    {
        // voxels
        // Use the voxel shader program
        glUseProgram(vox.m_program);

        // Set View and Perspective in shader
        glUniformMatrix4fv(glGetUniformLocation(vox.m_program, "view"), 1, GL_FALSE, glm::value_ptr(input.View()));
        glUniformMatrix4fv(glGetUniformLocation(vox.m_program, "projection"), 1, GL_FALSE, glm::value_ptr(input.Perspective()));

        // Bind the texture atlas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_atlas.TexName());

        vox.RenderInstancedGL(cubePositions.size());

        // skybox
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glUseProgram(skybox.m_program);
        glUniform1i(glGetUniformLocation(skybox.m_program, "skybox"), 0);

        glUniformMatrix4fv(glGetUniformLocation(skybox.m_program, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(input.View()))));
        glUniformMatrix4fv(glGetUniformLocation(skybox.m_program, "projection"), 1, GL_FALSE, glm::value_ptr(input.Perspective()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex.TexName());
        glDepthFunc(GL_LEQUAL);
        skybox.RenderGL();
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }

    float Application::hilly_terrain(float x, float y, float scale, float param_1, float param_2, float param_3) 
    {
        float x_scaled = x * scale;
        float y_scaled = y * scale;
        return std::sin(param_1 * x_scaled) * std::sin(param_2 * y_scaled) + std::exp(param_3 * ((x_scaled - 0.5) * (x_scaled - 0.5) + (y_scaled - 0.5) * (y_scaled - 0.5)));
    }

    std::vector<glm::vec3> Application::generate_voxel(int horz_axis_res, float vert_axis_ratio, float scale, int depth, float param_1, float param_2, float param_3) 
    {
        int vert_axis_res = static_cast<int>(horz_axis_res * vert_axis_ratio);

        std::vector<float> x_range(horz_axis_res);
        std::vector<float> y_range(horz_axis_res);
        for (int i = 0; i < horz_axis_res; ++i) {
            x_range[i] = -1 + 2 * static_cast<float>(i) / (horz_axis_res - 1);
            y_range[i] = -1 + 2 * static_cast<float>(i) / (horz_axis_res - 1);
        }

        std::vector<std::vector<float>> z(horz_axis_res, std::vector<float>(horz_axis_res));
        float z_min = std::numeric_limits<float>::max();
        float z_max = std::numeric_limits<float>::min();
        for (int i = 0; i < horz_axis_res; ++i) {
            for (int j = 0; j < horz_axis_res; ++j) {
                z[i][j] = hilly_terrain(x_range[i], y_range[j], scale, param_1, param_2, param_3);
                z_min = std::min(z_min, z[i][j]);
                z_max = std::max(z_max, z[i][j]);
            }
        }

        std::vector<std::vector<int>> z_index(horz_axis_res, std::vector<int>(horz_axis_res));
        for (int i = 0; i < horz_axis_res; ++i) {
            for (int j = 0; j < horz_axis_res; ++j) {
                z[i][j] = (z[i][j] - z_min) / (z_max - z_min);
                z_index[i][j] = Clamp(static_cast<int>(std::round(z[i][j] * vert_axis_res)) - 1, 1, vert_axis_res);
            }
        }

        std::vector<std::vector<std::vector<int>>> voxel(horz_axis_res, std::vector<std::vector<int>>(horz_axis_res, std::vector<int>(vert_axis_res, 0)));
        for (int i = 0; i < horz_axis_res; ++i) {
            for (int j = 0; j < horz_axis_res; ++j) {
                for (int k = std::max(z_index[i][j] - depth, 0); k < z_index[i][j]; ++k) {
                    voxel[i][j][k] = 1;
                }
            }
        }

        std::vector<glm::vec3> result;
        for (int i = 0; i < horz_axis_res; ++i) {
            for (int j = 0; j < horz_axis_res; ++j) {
                for (int k = 0; k < vert_axis_res; ++k) {
                    if (voxel[i][j][k] == 1) {
                        result.push_back(glm::vec3(i, k, j));
                    }
                }
            }
        }

        return result;
    }

    template <typename T>
    T Application::Clamp(const T& value, const T& minValue, const T& maxValue)
    {
        return (std::min)(maxValue, (std::max)(minValue, value));
    }



    GLuint Application::LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
    {
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open()) {
            std::string Line = "";
            while (getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }
        else {
            //        TRACE("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            return 0;
        }

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::string Line = "";
            while (getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }



        GLint Result = GL_FALSE;
        int InfoLogLength;



        // Compile Vertex Shader
    //    TRACE("Compiling shader : %s\n", vertex_file_path);
        char const* VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            //        TRACE("%s\n", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
    //    TRACE("Compiling shader : %s\n", fragment_file_path);
        char const* FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            //        TRACE("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
    //    TRACE("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            //        TRACE("%s\n", &ProgramErrorMessage[0]);
        }

        //Mark the shaders for deletion when no longer in use.
        //glDeleteShader(VertexShaderID);
        //glDeleteShader(FragmentShaderID);
        glUseProgram(ProgramID);

        return ProgramID;
    }
}
