#pragma once

// thirdparty
#include <glad/glad.h>
#include <vector>

#include "core/rendering/vertex.h"

// TODO: idk bro, i won't change this for now

struct VB_Element
{
    unsigned int type;
    unsigned int count;
    unsigned char normalised;

    bool vertex;
    size_t vertex_offset;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type) 
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VB_Vertex_Layout
{
private:
    std::vector<VB_Element> m_Elements;
    GLuint m_Stride = sizeof(Vertex);

public:
    template<typename T>
    void push(GLuint count, size_t vertex_offset)
    {
        __debugbreak();
    }

    template<>
    void push<GLfloat>(GLuint count, size_t vertex_offset)
    {
        m_Elements.push_back({ GL_FLOAT, count, GL_FALSE, true, vertex_offset });
    }

    inline const std::vector<VB_Element> getElements() const { return m_Elements; }
    inline GLuint getStride() const { return m_Stride; }
};