#include "Gui.hpp"

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

    terrainProperties.colorVec.r = terrainProperties.color[0];
    terrainProperties.colorVec.g = terrainProperties.color[1];
    terrainProperties.colorVec.b = terrainProperties.color[2];

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