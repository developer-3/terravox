#include "CubeTexture.h"

#include <vector>

tvox::CubeTexture::~CubeTexture()
{
}

bool tvox::CubeTexture::LoadFiles(const char* filename0, const char* filename1, const char* filename2, const char* filename3, const char* filename4, const char* filename5)
{
	std::vector<const char*> files;
	files.push_back(filename0);
	files.push_back(filename1);
	files.push_back(filename2);
	files.push_back(filename3);
	files.push_back(filename4);
	files.push_back(filename5);

	for (int i = 0; i < 6; i++) {
		if (m_textures[i].LoadFile(files[i]) == false)
			return false;
	}

	return true;
}

GLuint tvox::CubeTexture::TexName()
{
	if (initialized)
		return m_texname;

	glGenTextures(1, &m_texname);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texname);
	for (GLuint i = 0; i < 6; i++)
	{
		image = m_textures[i].Image();
		width = m_textures[i].Width();
		height = m_textures[i].Height();
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	initialized = true;

	return m_texname;
}
