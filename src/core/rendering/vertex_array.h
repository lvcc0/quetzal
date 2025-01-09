#pragma once

// thirdparty
#include <glad/glad.h>

#include "core/rendering/vertex_buffer_layout.h"
#include "core/rendering/vertex_buffer.h"
#include "core/rendering/element_buffer.h"

// Vertex Array Object
class VAO
{
public:
    VAO();
    ~VAO();

    GLuint getID() const;

    void addBuffer(const VBO& vbo, const VertexBufferLayout& layout) const;
    
    void bind() const;
    void unbind() const;

private:
    GLuint ID;
};