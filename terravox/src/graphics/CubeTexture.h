#pragma once

#include "Texture.h"


namespace tvox {
	class CubeTexture
	{
	public:
		virtual ~CubeTexture();

		bool LoadFiles(const char* filename0, const char* filename1, const char* filename2,
			const char* filename3, const char* filename4, const char* filename5);

		GLuint TexName();

	private:
		bool initialized;
		GLuint m_texname;

		Texture m_textures[6];
	};
}

