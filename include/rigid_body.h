#pragma once

#include "model.h"
#include "collision.h"

class RigidBody
{
public:
    std::shared_ptr<Model> m_Model;
    Collision m_Collision;

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);        // position in the world space
    glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);           // body scaling in the world space
    glm::vec3 m_RotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f); // body rotation in all 3 axes respectively in the world space

    // Abstarct variables for physics
    glm::vec3 m_MoveVector = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat m_Mass = 1.0f;

    // Constructor
    RigidBody(std::shared_ptr<Model>& model, Collision& collision);

    // Draw body
    void draw(std::shared_ptr<Shader>& shader);

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);

    void move();
};