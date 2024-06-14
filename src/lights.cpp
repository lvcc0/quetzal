#include "lights.h"

// --- Dir Light --- //
void DirLight::updateUni(std::shared_ptr<Shader>& shader, int index)
{
    std::string name = "dirLights[" + std::to_string(index) + "]";

    shader->setVec3(name + ".direction", m_dir);

    shader->setVec3(name + ".ambient", m_ambient);
    shader->setVec3(name + ".diffuse", m_diffuse);
    shader->setVec3(name + ".specular", m_specular);

    shader->setVec3(name + ".color", m_enabled ? m_color : glm::vec3(0.0f));
}
// --- //

// --- Point Light --- //
void PointLight::updateUni(std::shared_ptr<Shader>& shader, int index)
{
    std::string name = "pointLights[" + std::to_string(index) + "]";

    shader->setVec3(name + ".position", m_pos);

    shader->setVec3(name + ".ambient", m_ambient);
    shader->setVec3(name + ".diffuse", m_diffuse);
    shader->setVec3(name + ".specular", m_specular);

    shader->setFloat(name + ".constant", m_constant);
    shader->setFloat(name + ".linear", m_linear);
    shader->setFloat(name + ".quad", m_quad);

    shader->setVec3(name + ".color", m_enabled ? m_color : glm::vec3(0.0f));
}

void PointLight::enableBillboard()
{
    m_draw_billboard = !m_draw_billboard;
}
// --- //

// --- Spot Light --- //
void SpotLight::updateUni(std::shared_ptr<Shader>& shader, int index)
{
    std::string name = "spotLights[" + std::to_string(index) + "]";

    shader->setVec3(name + ".position", m_pos);
    shader->setVec3(name + ".direction", m_dir);

    shader->setVec3(name + ".ambient", m_ambient);
    shader->setVec3(name + ".diffuse", m_diffuse);
    shader->setVec3(name + ".specular", m_specular);

    shader->setFloat(name + ".constant", m_constant);
    shader->setFloat(name + ".linear", m_linear);
    shader->setFloat(name + ".quad", m_quad);
    
    shader->setFloat(name + ".innerCutoff", glm::cos(glm::radians(m_innerCutoff)));
    shader->setFloat(name + ".outerCutoff", glm::cos(glm::radians(m_outerCutoff)));
    
    shader->setVec3(name + ".color", m_enabled ? m_color : glm::vec3(0.0f));
}

void SpotLight::enableBillboard()
{
    m_draw_billboard = !m_draw_billboard;
}
// --- //