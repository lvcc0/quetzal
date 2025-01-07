#pragma once

#include <vector>
#include "renderer.h"
#include "buffers_objects.h"
#include "buffer_layout.h"

class VAO
{
public:
    VAO();
    ~VAO();

    void addBuffer(const VBO& vbo, const VB_Vertex_Layout& layout);

    void bind() const;
    void unbind() const;
    inline GLuint getID() const { return m_ID; }
private:
    GLuint m_ID;
};