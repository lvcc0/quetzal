#pragma once

// thirdparty
#include <glad/glad.h>

// Vertex Buffer Object
class VBO
{
public:
    VBO(const void* data, GLuint size);
    ~VBO();

    void bind() const;
    void unbind() const;

private:
    GLuint ID;
};