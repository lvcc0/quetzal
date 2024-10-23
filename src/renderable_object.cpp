#include "renderable_object.h"

Renderable::Renderable(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
	: Node(name), m_Vertices(vertices), m_Indices(indices)
{
	
}

Renderable::Renderable(const Renderable& obj)
	: Node(obj.getName()), m_Vertices(obj.m_Vertices), m_Indices(obj.m_Indices)
{
}

Renderable::~Renderable()
{
	vao_ptr = nullptr;
	vbo_ptr = nullptr;
	ibo_ptr = nullptr;
}

GLuint Renderable::getVAO() const
{
	return this->vao_ptr->getID();
}

glm::mat4 Renderable::getModelMatrix() const
{
	return m_ModelMatrix;
}
