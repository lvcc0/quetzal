#include "scene/resources/shader.h"

namespace qtzl
{
    Shader::Shader(const std::string& name, const std::string& path, const std::string& code, GLenum shader_type)
        : Resource(name, path)
    {
        this->m_Type = Object::Type::SHADER;

        const char* shaderSrc = code.c_str();

        this->ID = glCreateShader(shader_type);
        glShaderSource(this->ID, 1, &shaderSrc, NULL);
        glCompileShader(this->ID);
    }

    GLuint Shader::getID() const
    {
        return this->ID;
    }
}