//
//  application.cpp
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#include "application.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace vpr {

    vpr::GUI Application::gui;
    vpr::Input Application::input;

    void Application::run() {
        glClearColor(0.1, 0.1, 0.1, 1);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, WIDTH, HEIGHT);

        gui.gui_init(window.getWindow());

        glfwSetMouseButtonCallback(window.getWindow(), Application::mouse_button_callback);
        glfwSetCursorPosCallback(window.getWindow(), Application::cursor_position_callback);
        glfwSetScrollCallback(window.getWindow(), Application::scroll_callback);

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
        mesh_tex.LoadFile("src/textures/wavy.png");

        cylinder.LoadOBJ("src/models/cylinder1.obj");
        cylinder.m_program = LoadShaders("src/shaders/coordinates.vs", "src/shaders/transform.fs");
        cylinder.InitGL();

        box.CreateCube();
        box.m_program = LoadShaders("src/shaders/coordinates.vs", "src/shaders/transform.fs");
        box.InitGL();

        glClearColor(0.2, 0.6, 0.4, 1.);


        while (!window.shouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // per-frame time logic
            input.PerFrame();

            // Process input
            input.ProcessInput(window.getWindow());

            // gui render
            /*gui.gui_start();
            gui.render();
            gui.gui_end();*/
            
            // render
            RenderGL();

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
        input.CursorPosition(xpos, ypos);
    }

    void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        input.Scroll(xoffset, yoffset);
    }

    void Application::RenderGL()
    {

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(1.0f, -4.0f, 2.0f),
            glm::vec3(-1.5f, -2.2f, -3.0f),
            glm::vec3(-1.7f, 3.0f, -7.0f),
        };

        glUseProgram(box.m_program);

        // camera spin
        //float radius = 10.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camY = cos(glfwGetTime()) * radius;


        // Set View and Perspective in shader
        glUniformMatrix4fv(glGetUniformLocation(box.m_program, "view"), 1, GL_FALSE, glm::value_ptr(input.View()));
        glUniformMatrix4fv(glGetUniformLocation(box.m_program, "projection"), 1, GL_FALSE, glm::value_ptr(input.Perspective()));

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh_tex.TexName());
        
        // Draw each cube at a different location
        for (unsigned int i = 0; i < 5; i++)
        {
            glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[i]), glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(box.m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            box.RenderGL();
        }
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
