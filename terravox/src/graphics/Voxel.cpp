#include "Voxel.h"

#include "json.hpp"
using namespace nlohmann;

#include "gtc/type_ptr.hpp"

namespace tvox {
	Voxel::Voxel(int v_id) : voxel_id{v_id}
	{
        std::ifstream f("src/textures/atlases/blocks.json");
        json data = json::parse(f);
        // Access the values existing in JSON data
        json block = data[v_id];

        // get block id
        //auto block_id = id["block_id"].get<int>();

        // get tex coords from json
        auto texcoords = block["texture_coords"].get<std::vector<json>>();
        for (int i = 0; i < 6; i++)
        {
            auto u1 = texcoords[i]["u1"].get<float>();
            auto u2 = texcoords[i]["u2"].get<float>();
            auto v1 = texcoords[i]["v1"].get<float>();
            auto v2 = texcoords[i]["v2"].get<float>();

            texCoords.push_back(u1);
            texCoords.push_back(u2);
            texCoords.push_back(v1);
            texCoords.push_back(v2);
        }
	}

	Voxel::~Voxel(void)
	{
	}

    void Voxel::RenderGL()
    {
        glUseProgram(m_program);

        for (int i = 0; i < 6; i++)
        {
            glm::vec4 side(texCoords.at(i * 4), texCoords.at(i * 4 + 1), texCoords.at(i * 4 + 2), texCoords.at(i * 4 + 3));
            glUniform4f(glGetUniformLocation(m_program, sideNames[i]), side.x, side.y, side.z, side.w);
        }

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
    }

}

/*

- Get texcoords for each face
	- store texcoords in json?


- Write vertex shader to decide which texcoord to use, pass in 6 uniform vars with texcoords
    if (position.z == 0.5)  // Front face
        atlasTexCoord = vec2(texCoord.x, 1.0 - texCoord.y) * vec2(1.0/3.0, 0.5);
    else if (position.z == -0.5)  // Back face
        atlasTexCoord = vec2(texCoord.x, texCoord.y + 0.5) * vec2(1.0/3.0, 0.5);
    else if (position.y == 0.5)  // Top face
        atlasTexCoord = vec2(texCoord.x + 1.0/3.0, 1.0 - texCoord.y) * vec2(1.0/3.0, 0.5);
    else if (position.y == -0.5)  // Bottom face
        atlasTexCoord = vec2(texCoord.x + 2.0/3.0, texCoord.y + 0.5) * vec2(1.0/3.0, 0.5);
    else if (position.x == 0.5)  // Right face
        atlasTexCoord = vec2(texCoord.y + 1.0/2.0, 1.0 - texCoord.z) * vec2(1.0/3.0, 0.5);
    else if (position.x == -0.5)  // Left face
        atlasTexCoord = vec2(texCoord.y, 1.0 - texCoo

*/