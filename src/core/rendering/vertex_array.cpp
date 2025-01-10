#include "core/rendering/vertex_array.h"

VAO::VAO()
{
    glGenVertexArrays(1, &this->ID);
    this->bind();
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &this->ID);
}

GLuint VAO::getID() const
{
    return this->ID;
}

void VAO::addBuffer(const VBO& vbo, const VertexBufferLayout& layout) const
{
    this->bind();
    vbo.bind();

    const auto& elements = layout.getElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.size, element.type, element.normalized, layout.getStride(), (const void*)element.vertexOffset);
    }

    this->unbind();
    vbo.unbind();
}

void VAO::bind() const
{
    glBindVertexArray(this->ID);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}