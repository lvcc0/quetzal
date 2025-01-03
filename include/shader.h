#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <map>

#include "renderer.h"

#define ACTIVATE_SHADER(x);\
    if(!(is_active())) activateShader();\
    x

enum class ShaderType { MAIN, STENCIL, END };

// TODO: Maybe move this code to another file?
// 
// Auxillary function (its here mostly for using enums in vectors [])
template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

class Shader
{
public:
    GLuint ID;

    // Constructor
    Shader(std::string& vertexCode, std::string& fragmentCode);
    Shader(const Shader& obj) = delete;
    Shader(Shader&& obj) = delete;

    // Destructor
    ~Shader();

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

    void activateShader() const;
    void deleteShader() const;

protected:
    void compileErrors(unsigned int shader, const char* type);
    bool is_active() const;
};

struct Shaders_pack
{
    std::shared_ptr<Shader> MAIN_SHADER;
    std::shared_ptr<Shader> STENCIL_SHADER;

    std::shared_ptr<Shader> push(std::shared_ptr<Shader> shader, ShaderType type);
};