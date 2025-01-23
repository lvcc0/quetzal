#include "core/rendering/camera.h"

Camera::Camera(int width, int height)
    : m_Width(width), m_Height(height)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->m_Position, this->m_Position + m_Orientation, m_Up);
}

void Camera::processInput(GLFWwindow* window, float dt)
{
    // --- Keyboard --- //
    float velocity;

    (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? velocity = m_Speed * dt * 3 : velocity = m_Speed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->m_Position += velocity * this->m_Orientation;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->m_Position -= velocity * glm::normalize(glm::cross(this->m_Orientation, this->m_Up));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->m_Position -= velocity * this->m_Orientation;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->m_Position += velocity * glm::normalize(glm::cross(this->m_Orientation, this->m_Up));

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        this->m_Position += velocity * this->m_Up;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        this->m_Position -= velocity * this->m_Up;
    // --- //
    
    // --- Mouse --- //
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (this->m_FirstClick)
        {
            glfwSetCursorPos(window, (double)this->m_Width / 2, (double)this->m_Height / 2);
            this->m_FirstClick = false;
        }

        double mouseX, mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = this->m_Sensitivity * (float)(mouseY - (this->m_Height / 2)) / this->m_Height;
        float rotY = this->m_Sensitivity * (float)(mouseX - (this->m_Width / 2)) / this->m_Width;

        glm::vec3 newOrientation = glm::rotate(this->m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->m_Orientation, this->m_Up)));

        if (abs(glm::angle(newOrientation, this->m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
            this->m_Orientation = newOrientation;

        this->m_Orientation = glm::rotate(this->m_Orientation, glm::radians(-rotY), this->m_Up);

        glfwSetCursorPos(window, (double)this->m_Width / 2, (double)this->m_Height / 2);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        this->m_FirstClick = true;
    }
    // --- //
}

void Camera::updateSize(int width, int height)
{
    this->m_Width = width;
    this->m_Height = height;
}