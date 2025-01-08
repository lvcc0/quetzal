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
    Camera(int width, int height, glm::vec3 position = glm::vec3(0.0f));

    glm::vec3 m_Position;
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool m_FirstClick = true;

    int m_Width;
    int m_Height;

    // TODO: some options to change this stuff
    float m_Speed = 3.0f;
    float m_Sensitivity = 100.0f;

    // Returns current view matrix
    glm::mat4 getViewMatrix() const;
    
    // Handles keyboard and mouse inputs
    void processInput(GLFWwindow* window, float dt);
    
    // Updates camera's width and height
    void updateSize(int width, int height);
};