#include "buffers_objects.h"

VBO::VBO(const void* data, GLuint size)
{
    GLCall(glGenBuffers(1, &m_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VBO::~VBO()
{
    GLCall(glDeleteBuffers(1, &m_ID));
}

void VBO::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VBO::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IBO::IBO(const GLuint* data, GLuint count) :
	m_Count(count)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IBO::~IBO() 
{
	GLCall(glDeleteBuffers(1, &m_ID));
}

void IBO::bind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IBO::unbind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}