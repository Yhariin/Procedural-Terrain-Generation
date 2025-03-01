#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        MovementSpeed(SPEED), 
        MouseSensitivity(SENSITIVITY), 
        Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    firstMouse = true;
    updateCameraVectors();

}

Camera::Camera(float posX, 
        float posY, 
        float posZ, 
        float upX, 
        float upY, 
        float upZ, 
        float yaw, 
        float pitch) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        MovementSpeed(SPEED), 
        MouseSensitivity(SENSITIVITY), 
        Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();

}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_Speed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += glm::vec3(0.0, 1.0, 0.0) * velocity;
    if (direction == DOWN)
        Position -= glm::vec3(0.0, 1.0, 0.0) * velocity;
}

void Camera::ProcessMouseMovement(float x_offset, float y_offset, bool constrainPitch)
{
    x_offset *= MouseSensitivity;
    y_offset *= MouseSensitivity;

    Yaw   += x_offset;
    Pitch += y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float y_offset)
{
    Zoom -= (float)y_offset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;

}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}


CameraProperties Camera::getCameraProperties()
{
    CameraProperties cameraProperties;

    cameraProperties.firstMouse = firstMouse;
    cameraProperties.lastX = lastX;
    cameraProperties.lastY = lastY;
    cameraProperties.fov = fov;

    cameraProperties.Position = Position;
    cameraProperties.Front = Front;
    cameraProperties.Up = Up;
    cameraProperties.Right = Right;
    cameraProperties.WorldUp = WorldUp;

    cameraProperties.Yaw = Yaw;
    cameraProperties.Pitch = Pitch;

    cameraProperties.m_Speed = m_Speed;
    cameraProperties.MouseSensitivity = MouseSensitivity;
    cameraProperties.Zoom = Zoom;

    return cameraProperties;
}

void Camera::setCameraProperties(CameraProperties &cameraProperties)
{
    firstMouse = cameraProperties.firstMouse;
    lastX = cameraProperties.lastX;
    lastY = cameraProperties.lastY;
    fov = cameraProperties.fov;

    Position = cameraProperties.Position;
    Front = cameraProperties.Front;
    Up = cameraProperties.Up;
    Right = cameraProperties.Right;
    WorldUp = cameraProperties.WorldUp;

    Yaw = cameraProperties.Yaw;
    Pitch = cameraProperties.Pitch;

    m_Speed = cameraProperties.m_Speed;
    MouseSensitivity = cameraProperties.MouseSensitivity;
    Zoom = cameraProperties.Zoom;

}