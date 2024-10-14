#include "rigid_body.h"

RigidBody::RigidBody(const Model& model, Collision& collision) : Model (model), Collision (collision)
{
}


void RigidBody::translate(glm::vec3 vector)
{
    this->m_col_Position = m_Position;

    this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->m_Position);
}

void RigidBody::scale(glm::vec3 vector)
{
    this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, vector);

    this->m_col_Size *= vector;
}

void RigidBody::rotate(float degrees, glm::vec3 vector)
{
    // we don't rotate the collider, because it's either an AABB or a sphere
    this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, glm::radians(degrees), vector);
}