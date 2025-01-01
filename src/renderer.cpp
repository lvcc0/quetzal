#include "renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OPEN_GL::ERROR (" << error << "): " << function
            << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

Renderer& Renderer::instance()
{
    static Renderer instance;

    // TODO: do stuff here

    return instance;
}

void Renderer::draw(std::shared_ptr<Scene> scene, bool swap_buffers = true)
{
    // TODO: draw
}