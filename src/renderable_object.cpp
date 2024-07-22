#include "renderable_object.h"

Renderable::Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
{
	m_Vertices.swap(vertices);
	m_Indices.swap(indices);
}

Renderable::Renderable(const Renderable& obj)
	: m_Vertices(obj.m_Vertices), m_Indices(obj.m_Indices), VAO(obj.VAO), VBO(obj.VBO), EBO(obj.EBO)
{
	this->m_ModelMatrix = glm::mat4(1.0f);
}

Renderable::~Renderable()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (EBO != 0)
		glDeleteBuffers(1, &EBO);
}

GLuint Renderable::getVAO()
{
	return this->VAO;
}

glm::mat4 Renderable::getModelMatrix()
{
	return m_ModelMatrix;
}
