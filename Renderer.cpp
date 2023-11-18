#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer(uint32_t width, uint32_t height) : 
    m_TerrainGenerator(),
    m_Camera(glm::vec3(0.0f, 15.0f, 3.0f)), 
    m_shader("shaders/vert.glsl", "shaders/frag.glsl"),
    m_vao(),
    m_vbo(m_TerrainGenerator.getVertices().data(), m_TerrainGenerator.getVertices().size() * sizeof(float)),
    m_ibo(m_TerrainGenerator.getIndices().data(), m_TerrainGenerator.getIndices().size()),
    m_width(width),
    m_height(height)
{
    m_vao.AddBuffer(m_vbo, m_TerrainGenerator.getVertexBufferLayout());
}

Renderer::~Renderer()
{

}

void Renderer::Draw()
{
    m_shader.Bind();
    m_vao.Bind();
    m_ibo.Bind();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);
    glDrawElements(GL_TRIANGLE_STRIP, m_ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_PRIMITIVE_RESTART);

}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
    shader.Bind();
    va.Bind(); 
    ib.Bind();

    // UpdateCamera(shader);

}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Update()
{
    m_shader.Bind();

    float ratio = (float)m_width / (float)m_height;
    //upate any uniforms and matrices
    glm::mat4 m(1.0f);
    glm::mat4 v(1.0f);
    glm::mat4 p(1.0f);

    // m = glm::rotate(m, (float) glfwGetTime(), glm::vec3(0.f, 0.f, 1.0f));
    m = glm::translate(m, glm::vec3(-32.f, 0.f, -32.f));
    v = m_Camera.GetViewMatrix();
    // p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    p = glm::perspective(glm::radians(45.f), ratio, 0.1f, 100.0f);
    // p = glm::perspective(glm::radians(m_Camera.Zoom), ratio, 0.1f, 100.0f);

    // Order of matrix multiplication is reversed (read from right to left)
    // When creating the transformation matrix do, scaling -> rotations -> transformations
    // clip_vector = projection_matrix * view_matrix * transformation_matrix * local_vector
    glm::mat4 mvp =  p * v * m;

    m_shader.setMat4fv("MVP", mvp);
    m_shader.setVec3f("light_direction", glm::vec3(-1.0, 0.0, 0));

    // Lighting

    glm::vec3 lightPos(0.f, 10.f, 0.f);


}

void Renderer::UpdateCamera(Shader& shader)
{

    glm::mat4 view = m_Camera.GetViewMatrix();

    shader.setMat4fv("MVP", view);
}
