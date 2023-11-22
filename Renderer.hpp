#pragma once

#include "gfx.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "TerrainGenerator.hpp"
#include "Utils.hpp"

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
        Shader m_Shader;
        VertexArray m_Vao;
        VertexBuffer m_Vbo;
        IndexBuffer m_Ibo;

        uint32_t m_Width, m_Height;


        void UpdateCamera(Shader& shader);
        void Init();


};
