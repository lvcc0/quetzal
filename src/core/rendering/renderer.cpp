#include "core/rendering/renderer.h"

void Renderer::setCurrentShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program)
{
    m_CurrentShaderProgram = shader_program;
}

std::shared_ptr<ShaderProgram> Renderer::getCurrentShaderProgram()
{
    return m_CurrentShaderProgram;
}

glm::mat4 Renderer::getCurrentProjectionMatrix()
{
    return m_CurrentProjectionMatrix;
}

void Renderer::render(std::shared_ptr<Scene> scene)
{
    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);                                 // setting bg color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clearing stuff in the default framebuffer
    glStencilMask(0x00); // turn off writing to the stencil buffer

    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.deactivate();

    m_CurrentProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)scene->m_Camera.m_Width / (float)scene->m_Camera.m_Height, 0.1f, 100.0f);

    m_CurrentShaderProgram->activateProgram();
    
    m_CurrentShaderProgram->setVec3("viewPos", scene->m_Camera.m_Position);
    m_CurrentShaderProgram->setFloat("material.shininess", 32.0f);

    m_CurrentShaderProgram->setMat4("projection", m_CurrentProjectionMatrix);
    m_CurrentShaderProgram->setMat4("view", scene->m_Camera.getViewMatrix());

    // Rendering lights' influence
    if (!scene->getDirectionalLights().empty())
    {
        for (unsigned int i = 0; i < scene->getDirectionalLights().size(); i++)
        {
            scene->getDirectionalLights()[i]->updateUniforms(m_CurrentShaderProgram, i);
        }
    }
    if (!scene->getPointLights().empty())
    {
        for (unsigned int i = 0; i < scene->getPointLights().size(); i++)
        {
            scene->getPointLights()[i]->updateUniforms(m_CurrentShaderProgram, i);
        }
    }
    if (!scene->getSpotLights().empty())
    {
        for (unsigned int i = 0; i < scene->getSpotLights().size(); i++)
        {
            scene->getSpotLights()[i]->updateUniforms(m_CurrentShaderProgram, i);
        }
    }

    // Rendering renderable nodes
    for (const auto& node : scene->getNodes())
    {
        if (node->isRenderable())
            node->render(m_CurrentShaderProgram);
    }

    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.activate();
}