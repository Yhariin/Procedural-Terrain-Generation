#pragma once

#include <string>

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
    bool reloadEnabled;

    uint32_t prevWidth;
    uint32_t prevHeight;

    uint32_t prevPosX;
    uint32_t prevPosY;

    WindowProperties(const std::string& title = "Procedural Terrain Generation", uint32_t width = 1280, uint32_t height = 720, uint32_t pos_x = 0, uint32_t pos_y = 0) : 
    Title(title), Width(width), Height(height), posX(pos_x), posY(pos_y)
    {
    }

};