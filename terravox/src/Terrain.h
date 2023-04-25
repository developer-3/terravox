#pragma once

#include <vector>
using namespace std;

#include "glm.hpp"
#include "gtc/noise.hpp"

namespace tvox {
	class Terrain
	{
	public:

		static std::vector<glm::vec3> GeneratePerlinTerrain(int mapsize, float minHeight, float maxHeight);

		static float normalizeToRange(float value, float oldmin, float oldmax, float newmin, float newmax);

	};
}