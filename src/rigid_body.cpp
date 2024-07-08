#include "rigid_body.h"

RigidBody::RigidBody(std::shared_ptr<Model>& model, Collision& collision)
    : m_Model(model), m_Collision(collision)
{ /* empty */ }

void RigidBody::draw(std::shared_ptr<Shader>& shader)
{
    m_Model->draw(shader);

    translate(m_Position);
    scale(m_Scale);
    rotate(m_RotationDegrees.x, glm::vec3(1.0, 0.0, 0.0));
    rotate(m_RotationDegrees.y, glm::vec3(0.0, 1.0, 0.0));
    rotate(m_RotationDegrees.z, glm::vec3(0.0, 0.0, 1.0));
}

void RigidBody::move()
{
    m_Collision.m_Position += m_MoveVector;
    this->m_Position += m_MoveVector;

    m_Model->translate(m_MoveVector);
}

void RigidBody::translate(glm::vec3 vector)
{
    m_Collision.m_Position = vector;

    m_Model->m_ModelMatrix = glm::mat4(1.0f);
    m_Model->translate(vector);
}

void RigidBody::scale(glm::vec3 vector)
{
    m_Collision.m_Size *= vector;

    m_Model->scale(vector);
}

void RigidBody::rotate(float degrees, glm::vec3 vector)
{
    // we don't rotate the collider, because it's either an AABB or a sphere
    m_Model->rotate(degrees, vector);
}