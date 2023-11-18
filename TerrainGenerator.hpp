#pragma once

#include "gfx.hpp"
#include "glm/gtc/random.hpp"
#include "VertexBufferLayout.hpp"
#include "Utils.hpp"
#include <vector>
#include <list>

typedef struct VertexProperties {
    // float *obj;
    std::vector<float> obj;
    int obj_count;

    int size_pos;
    int offset_pos;

    int size_color;
    int offset_color;

    int size_normal;
    int offset_normal;

    int stride;
} VertexProperties;

class TerrainGenerator
{
    public:
        TerrainGenerator();
        ~TerrainGenerator();

        std::vector<float> getVertices() const { return m_Vertices; }
        std::vector<glm::vec3> getVertices_vec() const { return m_Vertices_vec; }
        std::vector<uint32_t> getIndices() const { return m_Indices; }
        VertexProperties getVertexProperties() const {return m_VertexProperties; }
        VertexBufferLayout getVertexBufferLayout() const {return m_VertexBufferLayout; }
    private:
        std::vector<float> m_Vertices; 
        std::vector<glm::vec3> m_Vertices_vec;
        std::vector<uint32_t> m_Indices; 
        std::vector<uint32_t> m_Indices_norm; 
        VertexProperties m_VertexProperties;
        VertexBufferLayout m_VertexBufferLayout;

        void computeVertexProperties(VertexProperties &VertexProperties, std::vector<float> &vertices);
        void GenerateChunk(int resolution);

};