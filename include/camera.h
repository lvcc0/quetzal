#pragma once

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
    glm::vec3 m_pos;
    glm::vec3 m_orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool m_firstClick = true;

    int m_width;
    int m_height;

    // TODO: some options to change this stuff
    float m_speed = 3.0f;
    float m_sens = 100.0f;

    // Constructor
    Camera(int width, int height, glm::vec3 pos);

    // Copy constructor
    Camera(const Camera& obj);

    // Returns current view matrix
    glm::mat4 getViewMatrix() const;
    
    // Handles keyboard and mouse inputs
    void Inputs(GLFWwindow* window, float dt);
    
    // Updates camera's width and height
    void UpdateSize(int w, int h);
};