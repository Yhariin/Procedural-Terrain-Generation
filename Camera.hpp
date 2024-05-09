#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraProperties.hpp"

#include <vector>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.f;
const float PITCH = 0.0f;
const float SPEED = 12.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
    public:
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
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // Construct with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
               float yaw = YAW, 
               float pitch = PITCH);

        Camera(float posX, 
               float posY, 
               float posZ, 
               float upX, 
               float upY, 
               float upZ, 
               float yaw, 
               float pitch);

        glm::mat4 GetViewMatrix() const;

        void ProcessKeyboard(Camera_Movement direction, float deltaTime);

        void ProcessMouseMovement(float x_offset, float y_offset, bool constrainPitch = true);

        void ProcessMouseScroll(float y_offset);

        float &getSpeed() { return m_Speed; }
        float &getSensitivity() { return m_Sensitivity; }

        void SetSpeed(float speed) { m_Speed = speed; }
        void setSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }
            
        CameraProperties getCameraProperties();
        void setCameraProperties(CameraProperties &cameraProperties);
    private:
        void updateCameraVectors();
        // float m_Yaw = -90.f;
        // float m_Pitch = 0.0f;
        float m_Speed = 50.0f;
        float m_Sensitivity = 0.1f;
        // float m_Zoom = 45.0f;
};