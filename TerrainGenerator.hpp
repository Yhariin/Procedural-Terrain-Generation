#pragma once

#include "gfx.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/noise.hpp"
#include "VertexBufferLayout.hpp"
#include "Utils.hpp"
#include <vector>
#include <map>
#include <list>

typedef struct VertexProperties {
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
        std::vector<glm::vec3> getVertices_vec() const { return m_VerticesVec3; }
        std::vector<uint32_t> getIndices() const { return m_Indices; }
        VertexProperties getVertexProperties() const {return m_VertexProperties; }
        VertexBufferLayout getVertexBufferLayout() const {return m_VertexBufferLayout; }

        void setColor(glm::vec3 color) { m_Color = color; }
        void setColor(float *color) {m_Color.x = color[0]; m_Color.y = color[1]; m_Color.z = color[2]; }
        glm::vec3 &getColor() { return m_Color; }
    private:
        std::vector<float> m_Vertices; 
        std::vector<glm::vec3> m_VerticesVec3;
        std::vector<glm::vec3> m_VertexColorsVec3;
        std::vector<uint32_t> m_Indices; 

        std::map<unsigned int, std::vector<int>> m_VertexToTrianglesMap;
        std::vector<glm::vec3> m_Triangles;

        std::vector<glm::vec3> m_TriangleNormals;
        std::vector<glm::vec3> m_VertexNormals;

        VertexProperties m_VertexProperties;
        VertexBufferLayout m_VertexBufferLayout;

        glm::vec3 m_Color = glm::vec3(0.3f, 0.3f, 0.3f);

        void computeVertexProperties(VertexProperties &VertexProperties, std::vector<float> &vertices);
        void GenerateChunk(int resolution, glm::vec3 color);
        void prepareVectors();
        void setVectorsAndBuffers();

};