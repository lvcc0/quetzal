#pragma once

// thirdparty
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera
{
public:
    Camera(int width, int height);

    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool m_FirstClick = true;
    bool m_Locked = false;

    int m_Width;
    int m_Height;

    float m_FOV = 45.0f;
    float m_Speed = 3.0f;
    float m_Sensitivity = 100.0f;

    // Returns current view matrix
    glm::mat4 getViewMatrix() const;
    
    // Handles keyboard and mouse inputs
    void processInput(GLFWwindow* window, float dt);
    
    // Updates camera's width and height
    void updateSize(int width, int height);
};