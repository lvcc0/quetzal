#pragma once

#include "scene/resources/shader.h"

// Shader program that got vertex and fragment shaders attached to it
class ShaderProgram
{
public:
    ShaderProgram(const std::string& name, unsigned int vertex_shader_id, unsigned int fragment_shader_id);
    ~ShaderProgram();

    // A bunch of functions to set shader uniforms

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    GLuint getID() const;
    std::string getName() const;

    void activateProgram() const;
    void deleteProgram() const;

protected:
    GLuint ID;
    std::string m_Name;

    void compileErrors(unsigned int shader_id, const std::string& type) const;
    bool isActive() const;
};