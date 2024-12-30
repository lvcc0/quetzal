#include "shader.h"

Shader::Shader(std::string& vertexCode, std::string& fragmentCode)
{
    const char* vShaderSrc = vertexCode.c_str();
    const char* fShaderSrc = fragmentCode.c_str();

    // SHADERS

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertexShader, 1, &vShaderSrc, NULL));
    GLCall(glCompileShader(vertexShader));

    compileErrors(vertexShader, "");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragmentShader, 1, &fShaderSrc, NULL));
    GLCall(glCompileShader(fragmentShader));

    compileErrors(fragmentShader, "");

    // SHADER PROGRAM

    ID = glCreateProgram();
    GLCall(glAttachShader(ID, vertexShader));
    GLCall(glAttachShader(ID, fragmentShader));

    glLinkProgram(ID);

    compileErrors(ID, "PROGRAM");

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
    deleteShader();
}

void Shader::activateShader() const
{
    #ifdef DEBUG
    std::cout << "DEBUG::SHADER::ACTIVATE " << ID << std::endl;
    #endif // DEBUG
    GLCall(glUseProgram(ID));
}

void Shader::deleteShader() const
{
    GLCall(glDeleteProgram(ID));
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "shader compilation error: " << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "shader linking error: " << type << "\n" << infoLog << std::endl;
        }
    }
}

bool Shader::is_active() const
{
    GLint shader_id;
    glGetIntegerv(GL_ACTIVE_PROGRAM, &shader_id);
    if (shader_id == this->ID)
        return true;

    return false;
}

void Shader::setBool(const std::string& name, bool value) const
{
    ACTIVATE_SHADER(GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value)));
}

void Shader::setInt(const std::string& name, int value) const
{
    ACTIVATE_SHADER(GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value)));
}

void Shader::setFloat(const std::string& name, float value) const
{
    ACTIVATE_SHADER(GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value)));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    ACTIVATE_SHADER(GLCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0])));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    ACTIVATE_SHADER(GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0])));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    ACTIVATE_SHADER(GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0])));
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const
{
    ACTIVATE_SHADER(GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0])));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
    ACTIVATE_SHADER(GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0])));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    ACTIVATE_SHADER(GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0])));
}

std::shared_ptr<Shader> Shaders_pack::push(std::shared_ptr<Shader> shader, ShaderType type)
{
    std::shared_ptr<Shader> p_shader{ nullptr };
    
    if (type == ShaderType::MAIN)
    {
        p_shader = MAIN_SHADER;
        MAIN_SHADER = shader;
    }
    else if (type == ShaderType::STENCIL)
    {
        p_shader = STENCIL_SHADER;
        STENCIL_SHADER = shader;
    }
    else
        ASSERT(false);

    return p_shader;
}