#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : m_Width(width), m_Height(height), m_Position(position)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::processInput(GLFWwindow* window, float dt)
{
    // --- Keyboard --- //
    float velocity;

    (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? velocity = m_Speed * dt * 3 : velocity = m_Speed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += velocity * m_Orientation;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= velocity * glm::normalize(glm::cross(m_Orientation, m_Up));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= velocity * m_Orientation;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += velocity * glm::normalize(glm::cross(m_Orientation, m_Up));

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        m_Position += velocity * m_Up;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        m_Position -= velocity * m_Up;
    // --- //
    
    // --- Mouse --- //
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (m_FirstClick)
        {
            glfwSetCursorPos(window, (double)m_Width / 2, (double)m_Height / 2);
            m_FirstClick = false;
        }

        double mouseX, mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = m_Sensitivity * (float)(mouseY - (m_Height / 2)) / m_Height;
        float rotY = m_Sensitivity * (float)(mouseX - (m_Width / 2)) / m_Width;

        glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

        if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
            m_Orientation = newOrientation;

        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

        glfwSetCursorPos(window, (double)m_Width / 2, (double)m_Height / 2);
    }
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_FirstClick = true;
    }
    // --- //
}

void Camera::updateSize(int weight, int height)
{
    m_Width = width;
    m_Height = height;
}