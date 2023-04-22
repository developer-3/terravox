#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 frontTexCoords;
uniform vec4 backTexCoords;
uniform vec4 topTexCoords;
uniform vec4 bottomTexCoords;
uniform vec4 rightTexCoords;
uniform vec4 leftTexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

    int x = int(aTexCoord.x);
    int y = int(aTexCoord.y);

    // Decide which texture to use depending on the face
	if (aNormal.z == 1.0)  // Front face
        TexCoord =  vec2(frontTexCoords[x], frontTexCoords[y+2]);
    else if (aNormal.z == -1.0)  // Back face
        TexCoord =  vec2(backTexCoords[x], backTexCoords[y+2]);
    else if (aNormal.y == -1.0)  // Top face
        TexCoord =  vec2(topTexCoords[x], topTexCoords[y+2]);
    else if (aNormal.y == 1.0)  // Bottom face
        TexCoord =  vec2(bottomTexCoords[x], bottomTexCoords[y+2]);
    else if (aNormal.x == 1.0)  // Right face
        TexCoord =  vec2(rightTexCoords[x], rightTexCoords[y+2]);
    else if (aNormal.x == -1.0)  // Left face
        TexCoord =  vec2(leftTexCoords[x], leftTexCoords[y+2]);

}