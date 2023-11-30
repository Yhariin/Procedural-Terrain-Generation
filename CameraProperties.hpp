#pragma once

#include "Utils.hpp"

struct CameraProperties
{
    // Additionals
    bool firstMouse;
    float lastX;
    float lastY;
    float fov;

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;
    
    // Camera Options
    float m_Speed;
    float MouseSensitivity;
    float Zoom;


};