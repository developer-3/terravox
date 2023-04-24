#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>

#include "glm.hpp"
#include <GLFW/glfw3.h>

#include "Mesh.h"

namespace tvox {
class Voxel : public CMesh
{
public:
    Voxel(int v_id);
    virtual ~Voxel(void);

    void InitGL(std::vector<glm::vec3> positions);
    void RenderGL() override;

    void RenderInstancedGL(int);

    unsigned int instanceVBO;

private:
    int voxel_id;

    std::vector<float> texCoords;
    std::vector<glm::vec3> positionArray;

    std::vector<const char *> sideNames{ "frontTexCoords", "backTexCoords", "topTexCoords", "bottomTexCoords", "rightTexCoords", "leftTexCoords" };
};
}