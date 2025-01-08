#include "core/rendering/shader_program.h"

ShaderProgram::ShaderProgram(const std::string&, unsigned int vertex_shader_id, unsigned int fragment_shader_id)
    : m_Name(name)
{
    this->compileErrors(vertex_shader_id, "");
    this->compileErrors(fragment_shader_id, "");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex_shader_id);
    glAttachShader(this->ID, fragment_shader_id);

    glLinkProgram(this->ID);

    this->compileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

ShaderProgram::~ShaderProgram()
{
    this->deleteProgram();
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& value) const
{
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint ShaderProgram::getID() const
{
    return this->ID;
}

std::string ShaderProgram::getName() const
{
    return this->m_Name;
}

void ShaderProgram::activateProgram() const
{
    glUseProgram(this->ID);
}

void ShaderProgram::deleteProgram() const
{
    glDeleteProgram(this->ID);
}

void ShaderProgram::compileErrors(unsigned int shader_id, const std::string& type) const
{
    GLint hasCompiled;
    char infoLog[1024];
    
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
            std::cerr << "shader compilation error: " << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader_id, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader_id, 1024, NULL, infoLog);
            std::cerr << "shader linking error: " << type << "\n" << infoLog << std::endl;
        }
    }
}

bool ShaderProgram::isActive() const
{
    GLint activeShaderID;
    glGetIntegerv(GL_ACTIVE_PROGRAM, &activeShaderID);
    
    return activeShaderID == this->ID;
}