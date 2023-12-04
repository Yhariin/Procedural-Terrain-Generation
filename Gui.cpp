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

    ImGui::Begin("SETTINGS");

    ImGui::Text("Camera Settings");
    ImGui::SliderFloat("Movement Speed", &camera.getSpeed(), 1.0f, 50.0f);


    ImGui::Text("Terrain Settings");

    ImGui::ColorPicker3("Terrain Color", terrainProperties.color);

    ImGui::SliderFloat("Scale", &terrainProperties.scale, 1.f, 1000.f);
    ImGui::SliderInt("Octaves", &terrainProperties.octaves, 1, 16);
    ImGui::SliderFloat("Persistance", &terrainProperties.persistance, 0.01f, 1.0f);
    ImGui::SliderFloat("Lacunarity", &terrainProperties.lacunarity, 0.01f, 3.0f);
    ImGui::SliderFloat("Height", &terrainProperties.height, 1.0f, 1000.f);
    ImGui::SliderFloat("Floor", &terrainProperties.floor, -100.f, 100.f);
    ImGui::SliderInt("Resolution", &terrainProperties.resolution, 4, 1024);

    ImGui::RadioButton("Perlin Noise", &terrainProperties.noiseType, 0);
    ImGui::RadioButton("Simplex Noise", &terrainProperties.noiseType, 1);


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

    ImGui::End();

    ImGui::Begin("Presets");

    ImGui::RadioButton("No Preset", &terrainProperties.preset, 0);
    ImGui::RadioButton("Preset 1", &terrainProperties.preset, 1);
    ImGui::RadioButton("Preset 2", &terrainProperties.preset, 2);
    ImGui::RadioButton("Preset 3", &terrainProperties.preset, 3);
    ImGui::RadioButton("Preset 4", &terrainProperties.preset, 4);
    ImGui::RadioButton("Preset 5", &terrainProperties.preset, 5);

    setTerrainPreset(terrainProperties);

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

void Gui::setTerrainPreset(TerrainProperties &terrainProperties)
{
    switch (terrainProperties.preset)
    {
    case 0:
        break;
    case 1:
        terrainProperties.scale = 700;
        terrainProperties.octaves = 16;
        terrainProperties.persistance = 0.561f;
        terrainProperties.lacunarity = 1.681f;
        terrainProperties.height = 100.f;
        terrainProperties.floor = 0.0f;
        terrainProperties.resolution = 256;
        terrainProperties.seed = 13065;
        terrainProperties.noiseType = terrainProperties.SIMPLEX;
        break;
    case 2:
        terrainProperties.scale = 700;
        terrainProperties.octaves = 16;
        terrainProperties.persistance = 0.605f;
        terrainProperties.lacunarity = 1.69f;
        terrainProperties.height = 1000.f;
        terrainProperties.floor = 0.0f;
        terrainProperties.resolution = 512;
        terrainProperties.seed = 26904;
        terrainProperties.noiseType = terrainProperties.PERLIN;
        break;
    case 3:
        terrainProperties.scale = 77;
        terrainProperties.octaves = 8;
        terrainProperties.persistance = 0.691f;
        terrainProperties.lacunarity = 3.00f;
        terrainProperties.height = 34.6f;
        terrainProperties.floor = 30.0f;
        terrainProperties.resolution = 128;
        terrainProperties.seed = 7257;
        terrainProperties.noiseType = terrainProperties.SIMPLEX;
        break;
    case 4:
        terrainProperties.scale = 200;
        terrainProperties.octaves = 8;
        terrainProperties.persistance = 0.300f;
        terrainProperties.lacunarity = 0.35f;
        terrainProperties.height = 15.0f;
        terrainProperties.floor = 0.0f;
        terrainProperties.resolution = 512;
        terrainProperties.seed = 27339;
        terrainProperties.noiseType = terrainProperties.SIMPLEX;
        break;
    case 5:
        terrainProperties.scale = 700;
        terrainProperties.octaves = 16;
        terrainProperties.persistance = 0.526f;
        terrainProperties.lacunarity = 1.75f;
        terrainProperties.height = 64.0f;
        terrainProperties.floor = 0.0f;
        terrainProperties.resolution = 128;
        terrainProperties.seed = 13065;
        terrainProperties.noiseType = terrainProperties.PERLIN;
        break;
    }

}