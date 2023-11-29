#include "Renderer.hpp"
#include <iostream>

float color [3];

Renderer::Renderer(GLFWwindow *window, uint32_t width, uint32_t height) : 
    m_TerrainGenerator(),
    m_Camera(glm::vec3(0.0f, 15.0f, 3.0f)), 
    m_Shader("shaders/vert.glsl", "shaders/frag.glsl"),
    m_Vao(),
    m_Vbo(m_TerrainGenerator.getVertices().data(), m_TerrainGenerator.getVertices().size() * sizeof(float)),
    m_Ibo(m_TerrainGenerator.getIndices().data(), m_TerrainGenerator.getIndices().size()),
    m_Gui(window),
    m_Width(width),
    m_Height(height)
{
    m_Vao.AddBuffer(m_Vbo, m_TerrainGenerator.getVertexBufferLayout());
}

Renderer::~Renderer()
{

}

void Renderer::Draw()
{
    m_Shader.Bind();
    m_Vao.Bind();
    m_Ibo.Bind();

    // glEnable(GL_PRIMITIVE_RESTART);
    // glPrimitiveRestartIndex(RESTART_INDEX);
    // glDrawElements(GL_TRIANGLE_STRIP, m_Ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    // glDisable(GL_PRIMITIVE_RESTART);

    glDrawElements(GL_TRIANGLES, m_Ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

    if(m_WindowProperties.guiEnabled)
        m_Gui.Render();
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

void Renderer::Update(WindowProperties &windowProperties)
{
    m_WindowProperties = windowProperties;
    m_Shader.Bind();

    float ratio = (float)m_Width / (float)m_Height;
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

    m_Shader.setMat4fv("model", m);
    m_Shader.setMat4fv("view", v);
    m_Shader.setMat4fv("projection", p);
    // m_Shader.setVec3f("light_direction", m_Camera.Front);
    m_Shader.setVec3f("light_direction", glm::normalize(glm::vec3(0.0, -1.0, 0.0)));

    glm::vec3 colorVec = m_TerrainGenerator.getColor();

    if(m_WindowProperties.guiEnabled)
        m_Gui.Update(m_Camera, color);


    colorVec.x = color[0];
    colorVec.y = color[1];
    colorVec.z = color[2];

    // Lighting

}

void Renderer::UpdateCamera(Shader& shader)
{
    glm::mat4 view = m_Camera.GetViewMatrix();

    shader.setMat4fv("MVP", view);
}
