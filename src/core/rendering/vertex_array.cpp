#include "vertex_array.h"

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

// TODO: yeah
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

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset);
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