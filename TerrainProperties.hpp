#pragma once

#include "Utils.hpp"
#include <iostream>

struct TerrainProperties
{
    enum 
    {
        PERLIN,
        SIMPLEX
    };

    TerrainProperties()
    {
        color[0] = 0.2f;
        color[1] = 0.2f;
        color[2] = 0.2f;

        colorVec = glm::vec3(0.2f, 0.2f, 0.2f);

        scale = 700.f;
        octaves = 16;
        persistance = 0.6f;
        lacunarity = 1.85f;
        height = 100.0f;
        floor = 0.0f;

        srand(time(0));
        seed = rand();

        resolution = 256;
        noiseType = 0;

        lightDirection[0] = 0.0f;
        lightDirection[1] = -1.0f;
        lightDirection[2] = -1.0f;

        lightDirectionVec = glm::vec3(0.0f, -1.0f, -1.0f);


        preset = 0;
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
    float floor;

    int resolution;
    int noiseType;

    int preset;
};