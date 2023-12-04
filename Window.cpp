#include "Window.hpp"
#include "TerrainProperties.hpp"

WindowProperties Window::m_WindowProperties;

GLFWwindow* Window::m_Handle = nullptr;

float Window::m_CurrentFrameTime = 0.f;
float Window::m_DeltaTime = 0.f;
float Window::m_LastFrameTime = 0.f;

void Window::Init_Window(const std::string& title, uint32_t width, uint32_t height, uint32_t posX, uint32_t posY)
{
    WindowProperties m_WindowProperties(title, width, height, posX, posY);

    m_WindowProperties.fullscreenEnabled = false;
    m_WindowProperties.wireframeEnabled = false;
    m_WindowProperties.guiEnabled = false;
    m_WindowProperties.reloadEnabled = false;

    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
    {
        cout << "Error initializing GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_Handle = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height, 
                                m_WindowProperties.Title.c_str(), NULL, NULL);

    glfwSetWindowPos(m_Handle, m_WindowProperties.posX, m_WindowProperties.posY);
    if(!m_Handle)
    {
        glfwTerminate();
        cout << "Error initializing window" << endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_Handle);
    glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Configure Callbacks
    glfwSetFramebufferSizeCallback(m_Handle, framebuffer_size_callback);
    glfwSetKeyCallback(m_Handle, key_callback);
    // glfwSetCursorPosCallback(window.handle, _cursor_callback);
    // glfwSetMouseButtonCallback(m_Handle, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Error initializing GLAD" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);


}


void Window::WindowLoop()
{
    TerrainProperties terrainProperties;
    Renderer* renderer;
    Camera *camera;
    renderer = new Renderer(m_Handle, terrainProperties, m_WindowProperties.Width, m_WindowProperties.Height);
    camera = renderer->getCamera();

    while(!glfwWindowShouldClose(m_Handle))
    {

        Window::m_CurrentFrameTime = (float)glfwGetTime();
        m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime;
        
        m_LastFrameTime = m_CurrentFrameTime;

        //TODO: Add FPS calculations here...

        glClearColor(0.1f, 0.1f, 0.1f, 1.f);

        renderer->Clear();

        renderer->Update(m_WindowProperties, terrainProperties);

        renderer->Draw();

        ProcessInputs(*camera);
        if(!m_WindowProperties.guiEnabled)
            ProcessMouse(*camera);

        glfwSwapBuffers(m_Handle);
        glfwPollEvents();

        if(m_WindowProperties.reloadEnabled)
        {
            CameraProperties cameraProperties = camera->getCameraProperties();
            delete renderer;
            renderer = new Renderer(m_Handle, terrainProperties, m_WindowProperties.Width, m_WindowProperties.Height);
            camera = renderer->getCamera();
            camera->setCameraProperties(cameraProperties);
            m_WindowProperties.reloadEnabled = false;
        }


    }

    delete renderer;

}


void Window::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        if(m_WindowProperties.guiEnabled)
        {
            // Toggle off
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_WindowProperties.guiEnabled = false;
        }
        else
        {
            // Toggle on
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_WindowProperties.guiEnabled = true;
        }

    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
    {
        if(m_WindowProperties.wireframeEnabled)
        {
            // Toggle off
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            m_WindowProperties.wireframeEnabled = false;
        }
        else
        {
            // Toggle on
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            m_WindowProperties.wireframeEnabled = true;
        }

    }
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        m_WindowProperties.reloadEnabled = true;
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {

        if(m_WindowProperties.fullscreenEnabled)
        {
            // Toggle off
            glfwRestoreWindow(m_Handle);
            m_WindowProperties.fullscreenEnabled = false;
        }
        else
        {
            // Toggle on
            glfwMaximizeWindow(m_Handle);
            m_WindowProperties.fullscreenEnabled = true;
        }

    }

}

/*
void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
*/

void Window::ProcessInputs(Camera& camera)
{
    if (glfwGetKey(m_Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Handle, true);
    if (glfwGetKey(m_Handle, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, m_DeltaTime);
    if (glfwGetKey(m_Handle, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, m_DeltaTime);
    if (glfwGetKey(m_Handle, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, m_DeltaTime);
    if (glfwGetKey(m_Handle, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, m_DeltaTime);
    if (glfwGetKey(m_Handle, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, m_DeltaTime);
    if (glfwGetKey(m_Handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, m_DeltaTime);

}


void Window::ProcessMouse(Camera& camera)
{
    double xposIn, yposIn;
    glfwGetCursorPos(m_Handle, &xposIn, &yposIn);

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top
    camera.lastX = xpos;
    camera.lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);

}
