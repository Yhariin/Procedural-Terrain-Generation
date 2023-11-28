#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Gui
{
    public:
        Gui(GLFWwindow *window);
        ~Gui();
        void Update();
        void Render();

    private:


};