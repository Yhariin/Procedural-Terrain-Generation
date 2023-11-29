#include "TerrainGenerator.hpp"
#include <iostream>


void TerrainGenerator::computeVertexProperties(VertexProperties &vertexProperties, std::vector<float> &vertices)
{
    vertexProperties.obj = vertices;
    vertexProperties.obj_count = vertices.size();
    // vertexProperties.obj_count = sizeof(vertices_t) / sizeof(float);

    vertexProperties.size_pos = 3;
    vertexProperties.offset_pos = 0;

    vertexProperties.size_color = 3;
    vertexProperties.offset_color = vertexProperties.size_pos * sizeof(float);

    vertexProperties.size_normal = 3;
    vertexProperties.offset_normal = vertexProperties.offset_color + (vertexProperties.size_color * sizeof(float));

    vertexProperties.stride = (vertexProperties.size_pos + vertexProperties.size_color) * 4;
}


TerrainGenerator::TerrainGenerator() 
{
    
    GenerateChunk(64, m_Color);

    computeVertexProperties(m_VertexProperties, m_Vertices);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_pos);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_color);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_normal);
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::GenerateChunk(int resolution, glm::vec3 color)
{
    m_VerticesVec3.reserve(resolution * resolution);
    float spacing = 1.0;
    float peak = 5.f;

    // Rows
    for(int r = 0; r < resolution; ++r)
    {
        // Columns
        for(int c = 0; c < resolution; ++c)
        {
            float x = c;
            float z = r;
            // float y = glm::linearRand(0.f, peak);
            float y = glm::perlin(glm::vec2(x * 0.2, z * 0.2)) * peak;
            // float y = 0.f;

            float normY = y / peak;

            glm::vec3 v(x * spacing, y * spacing, z * spacing);
            m_VerticesVec3.emplace_back(v);

            // Vertex Colors
            // m_VertexColorsVec3.push_back(glm::vec3(normY * 0.5, normY * 0.25, normY));
            m_VertexColorsVec3.push_back(glm::vec3(0.3, 0.3, 0.3));

            // Create a vector in the mapping
            // To keep track of all the triangles
            std::vector<int> triangles;
            m_VertexToTrianglesMap.emplace(m_VerticesVec3.size() - 1, triangles);


        }
    }

    // Generate Faces

    // Rows (-1 for last)
    for(int r = 0; r < resolution - 1; ++r)
    {
        // Columns (-1 for last)
        for (int c = 0; c < resolution - 1; ++c)
        {
            // Upper triangle
            /*
                v0 -- v2
                |    /
                |  /
                v1
            */
            int f0_0 = (r * resolution) + c;
            int f0_1 = ((r + 1) * resolution) + c;
            int f0_2 = (r * resolution) + c + 1;
            
            glm::vec3 f0(f0_0, f0_1, f0_2);
            m_Triangles.push_back(f0);

            // Lower triangle
            /*
                      v2
                     / |
                   /   |
                v0 --- v1
            */
           int f1_0 = ((r + 1) * resolution) + c;
           int f1_1 = ((r + 1) * resolution) + c + 1;
           int f1_2 = (r * resolution) + c + 1;

           glm::vec3 f1(f1_0, f1_1, f1_2);
           m_Triangles.push_back(f1);
        }
    }

    prepareVectors();

    setVectorsAndBuffers();

}

void TerrainGenerator::prepareVectors()
{

    // Compute triangle normals
    m_TriangleNormals.reserve(m_Triangles.size());
    for(int i = 0; i < m_Triangles.size(); ++i)
    {
        int v0i = m_Triangles[i].x;
        int v1i = m_Triangles[i].y;
        int v2i = m_Triangles[i].z;
        
        // Link vertex to triangle
        m_VertexToTrianglesMap.at(v0i).push_back(i);
        m_VertexToTrianglesMap.at(v1i).push_back(i);
        m_VertexToTrianglesMap.at(v2i).push_back(i);

        glm::vec3 v0 = m_VerticesVec3[v0i];
        glm::vec3 v1 = m_VerticesVec3[v1i];
        glm::vec3 v2 = m_VerticesVec3[v2i];
        glm::vec3 tn = glm::triangleNormal(v0, v1, v2);
        m_TriangleNormals.emplace_back(tn);
    }

    // Compute vertex normals
    m_VertexNormals.reserve(m_VerticesVec3.size());
    for(int i = 0; i < m_VerticesVec3.size(); ++i)
    {
        std::vector<int> triangle_indices = m_VertexToTrianglesMap[i];
        glm::vec3 v_sum = glm::vec3(0.0f);

        for(int j : triangle_indices)
        {
            v_sum = v_sum + m_TriangleNormals[j];
        }

        v_sum = glm::normalize(v_sum);
        m_VertexNormals.emplace_back(v_sum);
    }

}

void TerrainGenerator::setVectorsAndBuffers()
{
    int stride = 9;
    m_Vertices.resize(m_VerticesVec3.size() * 9);
    
    for(int i = 0; i < m_VerticesVec3.size(); ++i)
    {
        m_Vertices[(i * stride)] = m_VerticesVec3[i].x;
        m_Vertices[(i * stride)+1] = m_VerticesVec3[i].y;
        m_Vertices[(i * stride)+2] = m_VerticesVec3[i].z;

        m_Vertices[(i * stride)+3] = m_VertexColorsVec3[i].x;
        m_Vertices[(i * stride)+4] = m_VertexColorsVec3[i].y;
        m_Vertices[(i * stride)+5] = m_VertexColorsVec3[i].z;

        m_Vertices[(i * stride)+6] = m_VertexNormals[i].x;
        m_Vertices[(i * stride)+7] = m_VertexNormals[i].y;
        m_Vertices[(i * stride)+8] = m_VertexNormals[i].z;
    }

    for (glm::vec3 triangle : m_Triangles)
    {
        m_Indices.push_back((uint32_t)(triangle.x));
        m_Indices.push_back((uint32_t)(triangle.y));
        m_Indices.push_back((uint32_t)(triangle.z));
    }


}