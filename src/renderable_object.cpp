#include "renderable_object.h"

Renderable::Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
{
	m_Vertices.swap(vertices);
	m_Indices.swap(indices);
}

Renderable::Renderable(const Renderable& obj)
	: m_Vertices(obj.m_Vertices), m_Indices(obj.m_Indices)
{
	this->m_ModelMatrix = glm::mat4(1.0f);
	
}

Renderable::~Renderable()
{
	vao_ptr = nullptr;
	vbo_ptr = nullptr;
	ibo_ptr = nullptr;
}

GLuint Renderable::getVAO()
{
	return this->vao_ptr->getID();
}

glm::mat4 Renderable::getModelMatrix()
{
	return m_ModelMatrix;
}
