#pragma once

#include "gfx.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "TerrainGenerator.hpp"

class Renderer
{
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();
        void Draw();
        void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
        void Clear() const;
        void Update();

        Camera& getCamera() { return m_Camera; }

    private:
        TerrainGenerator m_TerrainGenerator;
        Camera m_Camera;
        Shader m_shader;
        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;

        uint32_t m_width, m_height;


        void UpdateCamera(Shader& shader);
        void Init();


};
