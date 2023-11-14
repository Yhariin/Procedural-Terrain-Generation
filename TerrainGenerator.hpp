#pragma once

#include "gfx.hpp"
#include "glm/gtc/random.hpp"
#include "VertexBufferLayout.hpp"
#include <vector>

typedef struct VertexProperties {
    // float *obj;
    std::vector<float> obj;
    int obj_count;

    int size_pos;
    int offset_pos;

    int size_color;
    int offset_color;

    int stride;
} VertexProperties;

class TerrainGenerator
{
    public:
        TerrainGenerator();
        ~TerrainGenerator();

        std::vector<float> getVertices() const { return m_Vertices; }
        std::vector<uint32_t> getIndices() const { return m_Indices; }
        VertexProperties getVertexProperties() const {return m_VertexProperties; }
        VertexBufferLayout getVertexBufferLayout() const {return m_VertexBufferLayout; }
    private:
        std::vector<float> m_Vertices; 
        std::vector<uint32_t> m_Indices; 
        VertexProperties m_VertexProperties;
        VertexBufferLayout m_VertexBufferLayout;

        void computeVertexProperties(VertexProperties &VertexProperties, std::vector<float> &vertices);
        void GenerateChunk(int resolution);

};