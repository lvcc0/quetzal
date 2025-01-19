#pragma once

// std
#include <iostream>
#include <algorithm>
#include <vector>

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/scene.h"
#include "core/rendering/shader_program.h"

class Renderer
{
public:
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    inline static float m_FOV = 45.0f;

    static glm::mat4 getCurrentProjectionMatrix();
    static std::map<ShaderProgram::Type, std::shared_ptr<ShaderProgram>> getCurrentShaderPrograms();

    // Sets current shader program of its type
    static void setCurrentShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program);

    // Draw all the stuff on the scene
    static void render(std::shared_ptr<Scene>& scene);

private:
    inline static glm::mat4 m_CurrentProjectionMatrix;

    inline static std::map<ShaderProgram::Type, std::shared_ptr<ShaderProgram>> m_CurrentShaderPrograms; // Type, *ShaderProgram
};