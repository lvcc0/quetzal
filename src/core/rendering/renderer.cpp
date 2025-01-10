#include "renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "OPEN_GL::ERROR (" << error << "): " << function << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}

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
    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.deactivate();

    m_CurrentProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)scene->m_Camera.m_Width / (float)scene->m_Camera.m_Height, 0.1f, 100.0f);

    m_CurrentShaderProgram->activateProgram();
    
    m_CurrentShaderProgram->setVec3("viewPos", scene->m_Camera.m_Position);
    m_CurrentShaderProgram->setFloat("material.shininess", 32.0f);

    m_CurrentShaderProgram->setMat4("projection", m_CurrentProjectionMatrix);
    m_CurrentShaderProgram->setMat4("view", scene->m_Camera.getViewMatrix());

    // TODO: Rendering lights' influence
    //if (!scene->m_DirLights.empty())
    //{
    //    for (auto i = 0; i < scene->m_DirLights.size(); i++)
    //    {
    //        scene->m_DirLights[i]->updateUni(currentShader, i);
    //    }
    //}
    //if (!scene->m_PointLights.empty())
    //{
    //    for (auto i = 0; i < scene->m_PointLights.size(); i++)
    //    {
    //        scene->m_PointLights[i]->updateUni(currentShader, i);
    //        // TODO: draw the billboard
    //    }
    //}
    //if (!scene->m_SpotLights.empty())
    //{
    //    for (auto i = 0; i < scene->m_SpotLights.size(); i++)
    //    {
    //        scene->m_SpotLights[i]->updateUni(currentShader, i);
    //        // TODO: draw the billboard
    //    }
    //}

    // Rendering renderable nodes
    for (const auto& node : scene->getNodes())
    {
        if (node->isRenderable())
            node->render(m_CurrentShaderProgram);
    }

    if (scene->m_PostProcessing.isActive() && scene->m_IsPostProcessing)
        scene->m_PostProcessing.activate();
}