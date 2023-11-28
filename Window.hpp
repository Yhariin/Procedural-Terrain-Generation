#include "gfx.hpp"
#include "DeltaTime.hpp"
#include "Renderer.hpp"
#include <iostream>

#include <string>

using std::cout, std::endl;

struct WindowProperties
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    uint32_t posX;
    uint32_t posY;

    bool fullscreenEnabled;
    bool wireframeEnabled;
    bool guiEnabled;

    uint32_t prevWidth;
    uint32_t prevHeight;

    uint32_t prevPosX;
    uint32_t prevPosY;

    WindowProperties(const std::string& title = "Procedural Terrain Generation", uint32_t width = 1280, uint32_t height = 720, uint32_t pos_x = 0, uint32_t pos_y = 0) : 
    Title(title), Width(width), Height(height), posX(pos_x), posY(pos_y)
    {
    }

};
    
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