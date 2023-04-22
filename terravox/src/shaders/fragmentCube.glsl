#version 330

in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube cube;

void main()
{
	FragColor = texture(cube, TexCoord);
}