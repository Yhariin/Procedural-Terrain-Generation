#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Window.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;
const uint32_t POS_X = 500;
const uint32_t POS_Y = 300;

using std::cout, std::endl;

int main(void)
{
    Window::Init_Window("Procedural Terrain Generation", WIDTH, HEIGHT, POS_X, POS_Y);

    Window::WindowLoop();

    return EXIT_SUCCESS;
}
/*
void processInput(GLFWwindow *window, Camera &camera, float deltaTime);

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

void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
*/


// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
// {
//     float xpos = static_cast<float>(xposIn);
//     float ypos = static_cast<float>(yposIn);

//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

//     lastX = xpos;
//     lastY = ypos;

//     camera.ProcessMouseMovement(xoffset, yoffset);
// }
// Window window("title", 4, 4);
/*
int main(void)
{
    DeltaTime deltaTime;
    float lastFrameTime = 0.0f;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    VertexProperties vertexProperties;
    computeVertexProperties(vertexProperties);

    // glm::vec3 triangleNormal = glm::triangleNormal(glm::vec3(vertices[0], vertices[1], vertices[2]), glm::vec3(vertices[6], vertices[7], vertices[8]), glm::vec3(vertices[12], vertices[13], vertices[14]));
    // cout << "(" << triangleNormal.x << ", " << triangleNormal.y << ", " << triangleNormal.z << ")" << endl; 

    GLFWwindow* window;
 
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

 
    Renderer renderer;

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

        // Input
        processInput(window, camera, deltaTime);


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
        glm::mat4 v(1.0f);
        glm::mat4 p(1.0f);

        m = glm::rotate(m, (float) glfwGetTime(), glm::vec3(0.f, 0.f, 1.0f));
        // p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        v = camera.GetViewMatrix();
        p = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 100.0f);

        // Order of matrix multiplication is reversed (read from right to left)
        // When creating the transformation matrix do, scaling -> rotations -> transformations
        // clip_vector = projection_matrix * view_matrix * transformation_matrix * local_vector
        glm::mat4 mvp =  p * v * m;

        
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
*/