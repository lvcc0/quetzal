#include "vertex_array.h"

VAO::VAO()
{
	GLCall(glGenVertexArrays(1, &m_ID));
	this->bind();
}

VAO::~VAO()
{
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VAO::addBuffer(const VBO& vbo, const VB_Vertex_Layout& layout)
{
	this->bind();
	vbo.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		offset = element.vertex_offset;

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset));
	}
	this->unbind();
	vbo.unbind();
}

void VAO::bind() const
{
	GLCall(glBindVertexArray(m_ID));
}

void VAO::unbind() const
{
	GLCall(glBindVertexArray(0));
}
