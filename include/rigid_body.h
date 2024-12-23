#pragma once

#include "model.h"
#include "collision.h"

class RigidBody : public Model, public Collision
{
public:
    // Abstarct variables for physics
    glm::vec3 m_MoveVector = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat m_Mass = 1.0f;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

    // Constructor
    RigidBody(const Model& model, Collision& collision, std::string name = "");

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);
};