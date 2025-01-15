#pragma once

// thirdparty
#include <glad/glad.h>
#include <vector>

#include "core/rendering/vertex.h"

// A single vertex buffer layout element containing some vertex data
struct LayoutElement
{
    GLuint size;
    GLenum type;
    GLboolean normalized;

    size_t vertexOffset;

    static unsigned int getTypeSize(unsigned int type)
    {
        switch (type) 
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }

        return 0;
    }
};

// Layout of elements to use in glVertexAttribPointer()
class VertexBufferLayout
{
public:
    void push(GLuint size, size_t vertex_offset);

    std::vector<LayoutElement> getElements() const;
    GLuint                     getStride() const;

private:
    std::vector<LayoutElement> m_Elements;
    GLuint m_Stride = sizeof(Vertex);
};