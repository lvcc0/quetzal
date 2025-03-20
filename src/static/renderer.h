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

class RendererVisitor;

class Renderer
{
    friend RendererVisitor;
public:
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    static glm::mat4 getCurrentProjectionMatrix();
    static glm::mat4 getCurrentViewMatrix();
    static std::map<ShaderProgram::Type, std::shared_ptr<ShaderProgram>> getCurrentShaderPrograms();

    // Sets current shader program of its type
    static void setCurrentShaderProgram(const std::shared_ptr<ShaderProgram>&& shader_program);

    // Draw all the stuff on the scene
    static void renderBegin(std::shared_ptr<Scene>& scene);
    static void renderEnd(std::shared_ptr<Scene>& scene);
private:
    inline static glm::mat4 m_CurrentProjectionMatrix;
    inline static glm::mat4 m_CurrentViewMatrix;
    inline static std::map<ShaderProgram::Type, std::shared_ptr<ShaderProgram>> m_CurrentShaderPrograms; // Type, *ShaderProgram
};