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

    void Voxel::RenderInstancedGL(int numInstances)
    {
        glUseProgram(m_program);
        for (int i = 0; i < 6; i++)
        {
            glm::vec4 side(texCoords.at(i * 4), texCoords.at(i * 4 + 1), texCoords.at(i * 4 + 2), texCoords.at(i * 4 + 3));
            glUniform4f(glGetUniformLocation(m_program, sideNames[i]), side.x, side.y, side.z, side.w);
        }

        glBindVertexArray(m_vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, vertexArray.size(), numInstances);
    }

    void Voxel::InitGL(std::vector<glm::vec3> positions)
    {
        for (PTV v = m_triangles.begin(); v != m_triangles.end(); v++)
        {
            normalArray.push_back(m_normals[v->n]);
            vertexArray.push_back(m_vertices[v->v]);
            texArray.push_back(m_tvertices[v->t]);
        }
        positionArray = positions;

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // Vertex positions
        glGenBuffers(1, &m_vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec3),
            &vertexArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Normals
        glGenBuffers(1, &m_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3),
            &normalArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Texture coords
        glGenBuffers(1, &m_texVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
        glBufferData(GL_ARRAY_BUFFER, texArray.size() * sizeof(glm::vec2),
            &texArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        // Set up instancing 
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(glm::vec3), &positionArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(3, 1);


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