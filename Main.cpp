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