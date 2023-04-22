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

    void RenderGL() override;

private:
    int voxel_id;

    std::vector<float> texCoords;

    std::vector<const char *> sideNames{ "frontTexCoords", "backTexCoords", "topTexCoords", "bottomTexCoords", "rightTexCoords", "leftTexCoords" };
};
}