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

    computeVertexProperties(m_VertexProperties, m_Vertices);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_pos);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_color);
}

TerrainGenerator::~TerrainGenerator()
{

}
