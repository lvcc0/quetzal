#pragma once

#include <vector>
#include "renderer.h"

class VBO
{
public:
    VBO(const void* data, GLuint size);
    ~VBO();

    void bind() const;
    void unbind() const;

private:
    GLuint m_ID;
};

class IBO
{
public:
    IBO(const GLuint* data, GLuint count);
    ~IBO();

    void bind() const;
    void unbind() const;

    inline GLuint getCount() const { return m_Count; }

private:
    GLuint m_ID;
    GLuint m_Count;
};
