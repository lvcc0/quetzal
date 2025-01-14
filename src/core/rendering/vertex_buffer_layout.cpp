#include "core/rendering/vertex_buffer_layout.h"

void VertexBufferLayout::push(GLuint size, size_t vertex_offset)
{
    this->m_Elements.push_back({ size, GL_FLOAT, GL_FALSE, vertex_offset });
}

std::vector<LayoutElement> VertexBufferLayout::getElements() const
{
    return this->m_Elements;
}

GLuint VertexBufferLayout::getStride() const
{
    return this->m_Stride;
}