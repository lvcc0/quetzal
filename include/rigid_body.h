#pragma once

#include "model.h"
#include "collision.h"

class RigidBody
{
public:
    std::shared_ptr<Model> m_Model;
    Collision m_Collision;

    // Constructor
    RigidBody(std::shared_ptr<Model> &model, Collision &collision);

    void Draw(std::shared_ptr<Shader> &shader);

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);
};