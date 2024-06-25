#include "rigid_body.h"

RigidBody::RigidBody(std::shared_ptr<Model>& model, Collision& collision)
    : m_Model(model), m_Collision(collision)
{ /* empty */ }

void RigidBody::Draw(std::shared_ptr<Shader> &shader)
{
    m_Model->Draw(shader);
}

void RigidBody::translate(glm::vec3 vector)
{
    m_Model->translate(vector);
}

void RigidBody::scale(glm::vec3 vector)
{
    m_Model->scale(vector);
}

void RigidBody::rotate(float degrees, glm::vec3 vector)
{
    m_Model->rotate(degrees, vector);
}