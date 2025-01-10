#pragma once

// std
#include <iostream>
#include <algorithm>
#include <vector>

// thirdparty
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/scene.h"
#include "core/rendering/shader_program.h"

class Renderer
{
public:
    // Delete all constructors
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    static void setCurrentShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program);

    static std::shared_ptr<ShaderProgram> getCurrentShaderProgram();
    static glm::mat4                      getCurrentProjectionMatrix();

    // Draw all the stuff on the scene
    static void render(std::shared_ptr<Scene> scene);

private:
    inline static glm::mat4 m_CurrentProjectionMatrix;
    inline static std::shared_ptr<ShaderProgram> m_CurrentShaderProgram;
};