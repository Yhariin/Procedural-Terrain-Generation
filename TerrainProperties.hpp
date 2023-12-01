#pragma once

#include "Utils.hpp"
#include <iostream>

struct TerrainProperties
{
    TerrainProperties()
    {
        color[0] = 0.2f;
        color[1] = 0.2f;
        color[2] = 0.2f;

        colorVec = glm::vec3(0.2f, 0.2f, 0.2f);

        scale = 0.05f;
        octaves = 4;
        persistance = 0.88f;
        lacunarity = 1.65f;
        height = 10.0f;

        seed = rand();

        resolution = 128;

        lightDirection[0] = 0.0f;
        lightDirection[1] = -1.0f;
        lightDirection[2] = 0.0f;

        lightDirectionVec = glm::vec3(0.0f, -1.0f, 0.0f);
    }

    float color[3];
    glm::vec3 colorVec;

    float lightDirection[3];
    glm::vec3 lightDirectionVec;

    int seed;

    float scale;
    int octaves;
    float persistance;
    float lacunarity;
    float height;

    int resolution;

};