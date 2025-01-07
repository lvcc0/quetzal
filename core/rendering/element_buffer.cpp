#include "core/rendering/element_buffer.h"

EBO::EBO(const GLuint* data, GLuint count)
    : m_Count(count)
{
    glGenBuffers(1, &this->ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &this->ID);
}

GLuint EBO::getCount() const
{
    return this->m_Count;
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
}

void EBO::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}