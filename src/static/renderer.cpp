#include "static/renderer.h"

glm::mat4 Renderer::getCurrentProjectionMatrix()
{
    return m_CurrentProjectionMatrix;
}

std::map<ShaderProgram::Type, std::shared_ptr<ShaderProgram>> Renderer::getCurrentShaderPrograms()
{
    return m_CurrentShaderPrograms;
}

void Renderer::setCurrentShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program)
{
    if (std::find(ResourceManager::getShaderPrograms().begin(), ResourceManager::getShaderPrograms().end(), shader_program) == ResourceManager::getShaderPrograms().end())
    {
        std::cerr << "There is no shader program named " << shader_program->getName() << std::endl;
        return;
    }

    m_CurrentShaderPrograms.emplace(shader_program->getType(), shader_program);
}

void Renderer::render(std::shared_ptr<Scene>& scene)
{
    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);                                 // setting bg color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clearing stuff in the default framebuffer
    glStencilMask(0x00); // turn off writing to the stencil buffer

    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.deactivate();

    m_CurrentProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)scene->m_Camera.m_Width / (float)scene->m_Camera.m_Height, 0.1f, 100.0f);

    // kinda lengthy, maybe we'll rewrite it, but i kinda like it
    m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT)->activateProgram();
    
    m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT)->setVec3("viewPos", scene->m_Camera.m_Position);
    m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT)->setFloat("material.shininess", 32.0f);

    m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT)->setMat4("projection", m_CurrentProjectionMatrix);
    m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT)->setMat4("view", scene->m_Camera.getViewMatrix());

    // Rendering lights' influence
    if (!scene->getDirectionalLights().empty())
    {
        for (unsigned int i = 0; i < scene->getDirectionalLights().size(); i++)
        {
            scene->getDirectionalLights()[i]->updateUniforms(m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT), i);
        }
    }
    if (!scene->getPointLights().empty())
    {
        for (unsigned int i = 0; i < scene->getPointLights().size(); i++)
        {
            scene->getPointLights()[i]->updateUniforms(m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT), i);
        }
    }
    if (!scene->getSpotLights().empty())
    {
        for (unsigned int i = 0; i < scene->getSpotLights().size(); i++)
        {
            scene->getSpotLights()[i]->updateUniforms(m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT), i);
        }
    }

    // Rendering renderable nodes
    for (const auto& node : scene->getVisualNodes())
    {
        if (node->getBool("Visible"))
        {
            if (node->getType() == qtzl::Object::SKYBOX && m_CurrentShaderPrograms.contains(ShaderProgram::SKYBOX))
            {
                m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->activateProgram();

                m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->setMat4("projection", m_CurrentProjectionMatrix);
                m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->setMat4("view", glm::mat4(glm::mat3(scene->m_Camera.getViewMatrix())));

                node->render(m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX));

                continue;
            }

            node->render(m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
        }
    }

    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.activate();
}