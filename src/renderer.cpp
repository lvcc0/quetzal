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

Renderer::Renderer(int viewport_width, int viewport_height)
    : m_PostProcessing(std::make_shared<PostProcessing>(ResourceManager::getScreenShaders(), viewport_width, viewport_height)),
      m_ViewportWidth(viewport_width), m_ViewportHeight(viewport_height)
{
}

Renderer::~Renderer()
{
}

void Renderer::setCurrentShader(std::shared_ptr<Shader> shader)
{
    this->m_CurrentShader = shader;
}

void Renderer::setCurrentStencilShader(std::shared_ptr<Shader> shader)
{
    this->m_CurrentStencilShader = shader;
}

void Renderer::draw(bool swap_buffers = true)
{
    if (!m_PostProcessing->m_ActiveShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->deactivate();

    m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)this->m_Camera->m_width / (float)this->m_Camera->m_height, 0.1f, 100.0f);
    glm::mat4 view = this->m_Camera->getViewMatrix();


    // TODO: rewrite shader stuff
    // Default shader
    m_CurrentShader->activateShader();
    
    m_CurrentShader->setVec3("viewPos", this->m_Camera->m_pos);
    m_CurrentShader->setFloat("material.shininess", 32.0f);

    m_CurrentShader->setMat4("projection", m_ProjectionMatrix);
    m_CurrentShader->setMat4("view", view);

    // Stencil shader
    m_CurrentStencilShader->activateShader();

    m_CurrentStencilShader->setMat4("projection", m_ProjectionMatrix);
    m_CurrentStencilShader->setMat4("view", view);

    // Rendering

    // TODO: consider moving lights into renderer
    // Rendering lights' influence
    if (!this->m_DirLights.empty())
    {
        for (auto i = 0; i < this->m_DirLights.size(); i++)
        {
            this->m_DirLights[i]->updateUni(m_CurrentShader, i);
        }
    }
    if (!this->m_PointLights.empty())
    {
        for (auto i = 0; i < this->m_PointLights.size(); i++)
        {
            this->m_PointLights[i]->updateUni(m_CurrentShader, i);

            //std::string name = m_PointLights[i]->m_Name;
            //std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(*std::find_if(m_NodeVec.begin(), m_NodeVec.end(), [name](std::shared_ptr<Scene_Node> item) {if (item->getName() == name) return true; return false; }));
            //sph_bill->m_Position = m_PointLights[i]->m_pos;
            //sph_bill->m_Target = m_Camera->m_pos;
            //sph_bill->draw(shaders_active);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(m_CurrentShader, i);
            
            //std::string name = m_SpotLights[i]->m_name;
            //std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(*std::find_if(m_NodeVec.begin(), m_NodeVec.end(), [name](std::shared_ptr<Scene_Node> item) {if (item->getName() == name) return true; return false; }));
            //sph_bill->m_Position = m_PointLights[i]->m_pos;
            //sph_bill->m_Target = m_Camera->m_pos;
            //sph_bill->draw(shaders_active);
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

    if (!m_PostProcessing->m_ActiveShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->activate();
}

void Renderer::togglePostProcessing()
{
    this->m_IsPostProcessing = !this->m_IsPostProcessing;
}