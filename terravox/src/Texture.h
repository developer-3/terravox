#pragma once

#include <GLFW/glfw3.h>

namespace vpr {
class Texture
{
public:
	void LoadFile(const char* filename);
	GLuint TexName() { return tex_name; }
private:
	GLuint tex_name;
	int width, height, nrChannels;
};
}

