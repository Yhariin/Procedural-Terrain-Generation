#pragma once

#include "gfx.hpp"
#include "DeltaTime.hpp"
#include "Renderer.hpp"
#include "WindowProperties.hpp"

#include <iostream>
#include <string>

using std::cout, std::endl;

    
class Window
{
    public:
        static void Init_Window(const std::string& title, uint32_t width, uint32_t height, uint32_t posX, uint32_t posY);

        static void WindowLoop();


    private:
        static GLFWwindow *m_Handle;
        static WindowProperties m_WindowProperties;
        static float m_CurrentFrameTime, m_DeltaTime, m_LastFrameTime;

        static void ProcessInputs(Camera& camera);
        static void ProcessMouse(Camera& camera);

        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

};