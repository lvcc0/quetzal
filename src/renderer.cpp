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

void Renderer::draw(Scene& scene, bool swap_buffers)
{
    if (!scene.m_PostProcessing.m_ActiveShaders.empty() && scene.m_IsPostProcessing)
        scene.m_PostProcessing.deactivate();

    currentProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)scene.m_Camera.m_width / (float)scene.m_Camera.m_height, 0.1f, 100.0f);
    glm::mat4 view = scene.m_Camera.getViewMatrix();

    // Default shader
    currentShader->activateShader();
    
    currentShader->setVec3("viewPos", scene.m_Camera.m_pos);
    currentShader->setFloat("material.shininess", 32.0f);

    currentShader->setMat4("projection", currentProjectionMatrix);
    currentShader->setMat4("view", view);

    // Stencil shader
    currentStencilShader->activateShader();

    currentStencilShader->setMat4("projection", currentProjectionMatrix);
    currentStencilShader->setMat4("view", view);

    // Rendering lights' influence
    if (!scene.m_DirLights.empty())
    {
        for (auto i = 0; i < scene.m_DirLights.size(); i++)
        {
            scene.m_DirLights[i]->updateUni(currentShader, i);
        }
    }
    if (!scene.m_PointLights.empty())
    {
        for (auto i = 0; i < scene.m_PointLights.size(); i++)
        {
            scene.m_PointLights[i]->updateUni(currentShader, i);
            // TODO: draw the billboard
        }
    }
    if (!scene.m_SpotLights.empty())
    {
        for (auto i = 0; i < scene.m_SpotLights.size(); i++)
        {
            scene.m_SpotLights[i]->updateUni(currentShader, i);
            // TODO: draw the billboard
        }
    }

    // TODO: yeah
    // Draw all renderable
    //if (!this->m_RenderableVec.empty())
    //{
    //    for (auto it: m_RenderableVec)
    //    {
    //        auto object = it;
    //        if (typeid(*object) == typeid(SphericalBillboard))
    //        {
    //            std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(it);
    //            sph_bill->m_Target = m_Camera->m_pos;
    //            sph_bill->draw(shaders_active);
    //        }
    //        else if (typeid(*object) == typeid(CylindricalBillboard))
    //        {
    //            std::shared_ptr<CylindricalBillboard> cyl_bill = std::dynamic_pointer_cast<CylindricalBillboard>(it);
    //            cyl_bill->m_Target = m_Camera->m_pos;
    //            cyl_bill->draw(shaders_active);
    //        }
    //        else 
    //        {
    //            it->draw(shaders_active);
    //        }
    //    }
    //}

    if (!scene.m_PostProcessing.m_ActiveShaders.empty() && scene.m_IsPostProcessing)
        scene.m_PostProcessing.activate();
}