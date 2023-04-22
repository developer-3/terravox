#pragma once

#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include "glm.hpp"
#include <GLFW/glfw3.h>

class CMesh
{
public:
    CMesh(void);
    virtual ~CMesh(void);

    void InitGL();
    virtual void RenderGL();
    void CleanGL();

    void RenderTop();
    void RenderSideBottom();

    GLuint m_program;
    GLuint m_vao;
    GLuint m_vertexVBO;
    GLuint m_normalVBO;
    GLuint m_texVBO;

    GLuint VAO;

    void AddVertex(const glm::vec3& v) { m_vertices.push_back(v); }
    void AddNormal(const glm::vec3& n) { m_normals.push_back(n); }
    void AddTexCoord(const glm::vec2& t) { m_tvertices.push_back(t); }
    void AddTriangleVertex(int v, int n, int t);

protected:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_tvertices;

    std::vector<glm::vec3> vertexArray;
    std::vector<glm::vec3> normalArray;
    std::vector<glm::vec2> texArray;

    // A triangle vertex description
    struct TV
    {
        int     v;      // Vertex
        int     n;      // Normal
        int     t;      // Texture coordinate
    };

    typedef std::vector<TV> Triangles;
    typedef Triangles::iterator PTV;
    Triangles       m_triangles;
public:
    void AddFlatQuad(int a, int b, int c, int d, int n);
    void AddQuad(int a, int b, int c, int d);
    void ComputeSmoothNormals(void);
    void LoadOBJ(const char* filename);
    void CreateCube();
};