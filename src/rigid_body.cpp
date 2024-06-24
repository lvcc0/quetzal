#include "rigid_body.h"

RigidBody::RigidBody(std::shared_ptr<Model>& model, CollisionType type)
{
	this->m_model = model;
	this->m_collision = Collision(type, this->m_model->m_vertices);
}

void RigidBody::Draw(std::shared_ptr<Shader> shader)
{
	m_model->Draw(shader);
	m_collision.updateModelMatrix(m_model_matrix);
	m_model_matrix = glm::mat4(1.0f);
}

void RigidBody::translate(glm::vec3 vector)
{
	m_model_matrix = glm::translate(m_model_matrix, vector);
	m_model->translate(vector);
}

void RigidBody::scale(glm::vec3 vector)
{
	m_model_matrix = glm::scale(m_model_matrix, vector);
	m_model->scale(vector);
}

void RigidBody::rotate(float degrees, glm::vec3 vector)
{
	m_model_matrix = glm::rotate(m_model_matrix, glm::radians(degrees), vector);
	m_model->rotate(degrees, vector);
}