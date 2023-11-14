#include "TerrainGenerator.hpp"
#include <iostream>


void TerrainGenerator::computeVertexProperties(VertexProperties &vertexProperties, std::vector<float> &vertices)
{
    // vertexProperties.obj = vertices_t;
    vertexProperties.obj = vertices;
    vertexProperties.obj_count = vertices.size();
    // vertexProperties.obj_count = sizeof(vertices_t) / sizeof(float);

    vertexProperties.size_pos = 3;
    vertexProperties.offset_pos = 0;

    vertexProperties.size_color = 3;
    vertexProperties.offset_color = vertexProperties.size_pos * sizeof(float);

    vertexProperties.stride = (vertexProperties.size_pos + vertexProperties.size_color) * 4;
}


TerrainGenerator::TerrainGenerator() 
{
    /*
    m_Vertices = {
        -0.6f, -0.4f, 0.5f,      1.f, 0.f, 0.f,
         0.6f, -0.4f, 0.5f,      0.f, 1.f, 0.f,
         0.f,   0.6f, 0.5f,      0.f, 0.f, 1.f, 
        -0.6f, -0.4f, 0.5f,      1.f, 0.f, 0.f,
         0.9f,  0.3f, 0.3f,      0.f, 1.f, 0.f,
         0.f,   0.2f, 0.3f,      0.f, 0.f, 1.f 
    };

    m_Indices = {
        0, 1, 2, 0, 4, 5, 6
    };
    */
    GenerateChunk(64);

    computeVertexProperties(m_VertexProperties, m_Vertices);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_pos);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_color);
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::GenerateChunk(int resolution)
{
    float factor = 100.f / (float)(resolution - 1);
    float peak = 5.0f;

    
    for(int r = 0; r < resolution; ++r) //y
    {
        for(int c = 0; c < resolution; ++c) //x
        {
            float y = glm::linearRand(0.f, peak);
            float normY = y / peak;

            m_Vertices.push_back(c); // X
            m_Vertices.push_back(y); // Y (random)
            m_Vertices.push_back(r); // Z

            m_Vertices.push_back(normY * 0.5f); // R
            m_Vertices.push_back(normY * 0.25f); // G
            m_Vertices.push_back(normY); // B


        }


    }


    for(int i = 0; i < resolution - 1; ++i)
    {
        for(int j = 0; j < resolution; ++j)
        {
            m_Indices.push_back((i+1) * resolution + j);
            m_Indices.push_back(i * resolution + j);

        }
        m_Indices.push_back(RESTART_INDEX);

    }

}
