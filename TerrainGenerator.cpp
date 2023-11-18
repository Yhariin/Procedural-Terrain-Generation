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

    vertexProperties.size_normal = 3;
    vertexProperties.offset_normal = vertexProperties.offset_color + (vertexProperties.size_color * sizeof(float));

    vertexProperties.stride = (vertexProperties.size_pos + vertexProperties.size_color) * 4;
}


TerrainGenerator::TerrainGenerator() 
{
    
    GenerateChunk(64);

    computeVertexProperties(m_VertexProperties, m_Vertices);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_pos);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_color);
    m_VertexBufferLayout.Push<float>(m_VertexProperties.size_normal);
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::GenerateChunk(int resolution)
{
    float factor = 100.f / (float)(resolution - 1);
    float peak = 5.0f;

    // Vertex Buffer
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

            // Vertex Normals
            m_Vertices.push_back(0.f);
            m_Vertices.push_back(0.f);
            m_Vertices.push_back(0.f);

            m_Vertices_vec.push_back(glm::vec3(c, y, r));
            // m_Vertices_vec.push_back(glm::vec3(normY * 0.5f, normY * 0.25f, normY));

        }


    }

    // Index Buffer
    for(int i = 0; i < resolution - 1; ++i)
    {
        for(int j = 0; j < resolution; ++j)
        {
            m_Indices.push_back((i+1) * resolution + j);
            m_Indices.push_back(i * resolution + j);

            m_Indices_norm.push_back((i+1) * resolution + j);
            m_Indices_norm.push_back(i * resolution + j);

        }
        m_Indices.push_back(RESTART_INDEX);

    }

    // Calculate Normals
    std::vector<std::list<glm::vec3>> vertex_normals;
    vertex_normals.reserve(m_Indices.size());
    for(int i = 0; i < m_Indices.size(); ++i)
    {
        std::list<glm::vec3> l;
        vertex_normals.push_back(l);
    }


    int stride = 3;
    int xyzCounter = 0;
    int vertexCounter = 0;
    int counter = 0;
    for(int i = 0; i < resolution - 1; ++i)
    {
        for(int j = 2; j < resolution * 2; ++j)
        {
            // counter = (i * resolution) + j;
            glm::vec3 A(m_Vertices_vec[m_Indices_norm[counter]]);
            glm::vec3 B(m_Vertices_vec[m_Indices_norm[counter+1]]);
            glm::vec3 C(m_Vertices_vec[m_Indices_norm[counter+2]]);

            glm::vec3 normal = glm::triangleNormal(A, B, C);

            vertex_normals[m_Indices_norm[counter]].push_back(normal);
            vertex_normals[m_Indices_norm[counter+1]].push_back(normal);
            vertex_normals[m_Indices_norm[counter+2]].push_back(normal);
            counter++;
            /*
            glm::vec3 A(m_Vertices[xyzCounter + (m_Indices[vertexCounter] * stride)], m_Vertices[(xyzCounter+1) + (m_Indices[vertexCounter] * stride)], m_Vertices[(xyzCounter+2) + (m_Indices[vertexCounter] * stride)]);
            xyzCounter += stride;
            vertexCounter++;
            glm::vec3 B(m_Vertices[xyzCounter + (vertexCounter * stride)], m_Vertices[(xyzCounter+1) + (vertexCounter * stride)], m_Vertices[(xyzCounter+2) + (vertexCounter * stride)]);
            xyzCounter += stride;
            vertexCounter++;
            glm::vec3 C(m_Vertices[xyzCounter + (vertexCounter * stride)], m_Vertices[(xyzCounter+1) + (vertexCounter * stride)], m_Vertices[(xyzCounter+2) + (vertexCounter * stride)]);

            glm::triangleNormal(A, B, C);

            xyzCounter -= (stride * 2) + 1;
            vertexCounter--;
            */
        }
        counter +=2;

    }

    std::vector<glm::vec3> vertex_normals_average;

    for(int i = 0; i < m_Indices.size(); ++i)
    {
        int count = 0;
        glm::vec3 sum(0, 0, 0);
        for(const auto& vec : vertex_normals[i])
        {
            sum += vec;
            count++;

        }
        // for(std::list<glm::vec3>::iterator it = vertex_normals[i].begin(); it != vertex_normals[i].end(); ++it)
        // {
        //     it;

        //     count++;
        // }
        vertex_normals_average.push_back(sum / (float)count);
    }

    stride = 6;
    m_Vertices[6] = vertex_normals_average[0].x;
    m_Vertices[7] = vertex_normals_average[0].y;
    m_Vertices[8] = vertex_normals_average[0].z;
    for(int i = 1; i < resolution * resolution; ++i)
    {
        m_Vertices[stride + (i * 9)] = vertex_normals_average[i].x;
        m_Vertices[stride + (i * 9) + 1] = vertex_normals_average[i].y;
        m_Vertices[stride + (i * 9) + 2] = vertex_normals_average[i].z;
    }
}
