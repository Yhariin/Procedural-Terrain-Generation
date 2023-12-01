#include "Gui.hpp"
#include <string>
#include <random>

Gui::Gui(GLFWwindow *window)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_ModifiableValues.color[0] = 0.3f;
    m_ModifiableValues.color[1] = 0.3f;
    m_ModifiableValues.color[2] = 0.3f;
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::Update(Camera &camera, TerrainProperties &terrainProperties)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("TEST WINDOW");

    ImGui::Text("Camera Settings");
    ImGui::SliderFloat("Movement Speed", &camera.getSpeed(), 1.0f, 25.0f);


    ImGui::Text("Terrain Settings");

    ImGui::ColorPicker3("Terrain Color", terrainProperties.color);

    ImGui::SliderFloat("Scale", &terrainProperties.scale, 0.01f, 0.2f);
    ImGui::SliderInt("Octaves", &terrainProperties.octaves, 1, 32);
    ImGui::SliderFloat("Persistance", &terrainProperties.persistance, 0.01f, 1.0f);
    ImGui::SliderFloat("Lacunarity", &terrainProperties.lacunarity, 0.01f, 5.0f);
    ImGui::SliderFloat("Height", &terrainProperties.height, 1.0f, 25.f);
    ImGui::SliderInt("Resolution", &terrainProperties.resolution, 3, 128);


    std::string seedButton = "Randomize Seed " + std::to_string(terrainProperties.seed);
    if(ImGui::Button(seedButton.c_str()))
        terrainProperties.seed = rand();

    ImGui::SliderFloat3("Light Direction", terrainProperties.lightDirection, -1.0f, 0.0f);


    terrainProperties.colorVec.r = terrainProperties.color[0];
    terrainProperties.colorVec.g = terrainProperties.color[1];
    terrainProperties.colorVec.b = terrainProperties.color[2];

    terrainProperties.lightDirectionVec.x = terrainProperties.lightDirection[0];
    terrainProperties.lightDirectionVec.y = terrainProperties.lightDirection[1];
    terrainProperties.lightDirectionVec.z = terrainProperties.lightDirection[2];

    ImGui::Text("HELLO");

    ImGui::End();
}

void Gui::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool Gui::isUpdated()
{
    if(m_ModifiableValues.lastColor[0] != m_ModifiableValues.color[0] ||
        m_ModifiableValues.lastColor[1] != m_ModifiableValues.color[1] ||
        m_ModifiableValues.lastColor[2] != m_ModifiableValues.color[2])
    {
        return true;
    }

    return false;

}