#pragma once

// thirdparty
#include <glad/glad.h>

// Element Buffer Object
class EBO
{
public:
    EBO(const GLuint* data, GLuint count);
    ~EBO();

    GLuint getCount() const;

    void bind() const;
    void unbind() const;

private:
    GLuint ID;
    GLuint m_Count;
};