#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
 
#include "Renderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include "Shader.hpp"
#include "DeltaTime.hpp" 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

using std::cout, std::endl;

float vertices [] = {
    -0.6f, -0.4f, 0.5f,      1.f, 0.f, 0.f,
     0.6f, -0.4f, 0.5f,      0.f, 1.f, 0.f,
     0.f,   0.6f, 0.5f,      0.f, 0.f, 1.f, 
    -0.6f, -0.4f, 0.5f,      1.f, 0.f, 0.f,
     0.9f,  0.3f, 0.3f,      0.f, 1.f, 0.f,
     0.f,   0.2f, 0.3f,      0.f, 0.f, 1.f 
};

float quad_vertices [] = {
    -1.0f, -1.0f, 1.0f,     1.f, 0.f, 0.f,
    -1.0f,  1.0f, 1.0f,     0.f, 1.f, 0.f,
     1.0f, -1.0f, 1.0f,     0.f, 0.f, 1.f,
     1.0f,  1.0f, 1.0f,     0.5f, 0.5f, 0.5f
};

unsigned int indices[] = {
    0, 1, 2, 0, 4, 5, 6
};

unsigned int quad_indices [] = {
    0, 1, 2, 2, 3, 1
};

typedef struct VertexProperties {
    float *obj;
    int obj_count;

    int size_pos;
    int offset_pos;

    int size_color;
    int offset_color;

    int stride;
} VertexProperties;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void computeVertexProperties(VertexProperties &vertexProperties)
{
    vertexProperties.obj = vertices;
    vertexProperties.obj_count = sizeof(vertices) / sizeof(float);

    vertexProperties.size_pos = 3;
    vertexProperties.offset_pos = 0;

    vertexProperties.size_color = 3;
    vertexProperties.offset_color = vertexProperties.size_pos * sizeof(float);

    vertexProperties.stride = (vertexProperties.size_pos + vertexProperties.size_color) * 4;
}

int main(void)
{
    DeltaTime deltaTime;
    float lastFrameTime = 0.0f;

    VertexProperties vertexProperties;
    computeVertexProperties(vertexProperties);

    // glm::vec3 triangleNormal = glm::triangleNormal(glm::vec3(vertices[0], vertices[1], vertices[2]), glm::vec3(vertices[6], vertices[7], vertices[8]), glm::vec3(vertices[12], vertices[13], vertices[14]));
    // cout << "(" << triangleNormal.x << ", " << triangleNormal.y << ", " << triangleNormal.z << ")" << endl; 

    GLFWwindow* window;
    int mvp_location;
 
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 
    window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural Terrain Generation", NULL, NULL);
    glfwSetWindowPos(window, 500, 300);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);
 
    // NOTE: OpenGL error checks have been omitted for brevity
 
    Shader shader("shaders/vert.glsl", "shaders/frag.glsl");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexArray va, quad_va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBuffer quad_vb(quad_vertices, sizeof(quad_vertices));
    VertexBufferLayout layout;

    layout.Push<float>(vertexProperties.size_pos);
    layout.Push<float>(vertexProperties.size_color);
    va.AddBuffer(vb, layout);
    quad_va.AddBuffer(quad_vb, layout);

    IndexBuffer ib(indices, sizeof(indices)/sizeof(unsigned int));
    ib.Bind();

    IndexBuffer quad_ib(quad_indices, sizeof(quad_indices) / sizeof(unsigned int));
    quad_ib.Bind();

    // mvp_location = glGetUniformLocation(shader.getProgramID(), "MVP");
 
    Renderer renderer;


    glm::ivec2 resolution = glm::ivec2(WIDTH, HEIGHT);

    int resolutionX = WIDTH;
    int resolutionY = HEIGHT;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        // Delta Time Calculations
        float currentFrameTime = (float)glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        
        lastFrameTime = currentFrameTime;



        glClearColor(0.f, 0.f, 0.f, 1.f);

        float ratio;
        int width, height;
        // glm::mat4 p, mvp;
 
        glfwGetFramebufferSize(window, &width, &height);
        ratio = (float)width / (float) height;
 
        glViewport(0, 0, width, height);

        renderer.Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        //TODO: learn model, view, and projection matrices properly
        //TODO: learn the order in which to multiply matrices
        glm::mat4 m(1.0f);
        glm::mat4 p(1.0f);

        m = glm::rotate(m, (float) glfwGetTime(), glm::vec3(0.f, 0.f, 1.0f));
        p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        glm::mat4 mvp = p * m;

        shader.setMat4fv("MVP", mvp);
        
        renderer.Draw(va, ib, shader);

        ImGui::Begin("IMGUI WINDOW");
        ImGui::Text("HELLO");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}