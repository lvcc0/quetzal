#pragma once

#include "collision.h"
#include "model.h"

class RigidBody {
public:
    std::shared_ptr<Model> m_model;
    Collision m_collision;
    // Constructors
    RigidBody(std::shared_ptr<Model>& model, CollisionType type);

    void Draw(std::shared_ptr <Shader> shader);

    glm::mat4 m_model_matrix = glm::mat4(1.0f);

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);
};