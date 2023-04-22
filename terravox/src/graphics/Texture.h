#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

namespace tvox {
class Texture
{
public:
	bool LoadFile(const char* filename);
	GLuint TexName() { return tex_name; }

	int Width() const;
	int Height() const;
	unsigned char* Image() const;

private:
	GLuint tex_name;
	int width, height, nrChannels;

	unsigned char* image;
};
}

