#include "Terrain.h"

#include "exponential.hpp"

namespace tvox {

    /** Generate terrain based on classic perlin noise
    * mapsize: how big the terrain will be NxN
    * minHeight: minimum height of land
    * maxHeight: maximum height of land
    */
    std::vector<glm::vec3> Terrain::GeneratePerlinTerrain(int mapsize, float minHeight, float maxHeight)
    {
        std::vector<glm::vec3> terrain;

        glm::vec2 frequency = glm::vec2(0.02f, 0.03f);
        glm::vec2 amplitude = glm::vec2(0.4f, 10.0f);

        int seed = 123;

        for (int x = 0; x < mapsize; x++)
        {
            for (int z = 0; z < mapsize; z++)
            {
                float perlinValue = glm::perlin(glm::vec2(x, z) * frequency, amplitude);
                int normalizedY = (int)normalizeToRange(perlinValue, -1.0f, 1.0f, minHeight, maxHeight);
                terrain.push_back(glm::vec3(x, normalizedY, z));

                for (int y = 0; y < normalizedY; y++)
                {
                    terrain.push_back(glm::vec3(x, y, z));
                }
            }
        }

        return terrain;
    }

    /** Normalize a value from an old range to a new range
    * ex: old range (-1, 1) to new range (0, 255)
    */
    float Terrain::normalizeToRange(float value, float oldmin, float oldmax, float newmin, float newmax)
    {
        return (((value - oldmin) * (newmax - newmin)) / (oldmax - oldmin)) + newmin;
    }
}