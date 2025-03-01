#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Camera.hpp"
#include "TerrainProperties.hpp"

class Gui
{
    public:
        Gui(GLFWwindow *window);
        ~Gui();
        void Update(Camera &camera, TerrainProperties &terrainProperties);
        void Render();

        bool isUpdated();

    private:
        typedef struct ModifiableValues
        {
            float color [3];
            float lastColor [3];

        } ModifiableValues;
        
        ModifiableValues m_ModifiableValues;

        void setTerrainPreset(TerrainProperties &terrainProperties);


};