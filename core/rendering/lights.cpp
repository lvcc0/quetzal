#include "lights.h"

Light::Light(bool enabled, const std::string& name, glm::vec3 color,
             glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : m_Enabled(enabled), m_Name(name), m_Color(color),
      m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular)
{
}

DirLight::DirLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 dir,
                   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(enabled, name, color, ambient, diffuse, specular), m_Direction(dir)
{
}

void DirLight::updateUni(std::shared_ptr<Shader>& shader, int index) const
{
    std::string name = "dirLights[" + std::to_string(index) + "]";

    shader->setVec3(name + ".direction", m_Direction);

    shader->setVec3(name + ".ambient", m_Ambient);
    shader->setVec3(name + ".diffuse", m_Diffuse);
    shader->setVec3(name + ".specular", m_Specular);

    shader->setVec3(name + ".color", m_Enabled ? m_Color : glm::vec3(0.0f));
}

PointLight::PointLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 pos,
                       glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                       float constant, float linear, float quad)
    : Light(enabled, name, color, ambient, diffuse, specular), m_Position(pos),
      m_Constant(constant), m_Linear(linear), m_Quad(quad)
{
}

void PointLight::updateUni(std::shared_ptr<Shader>& shader, int index) const
{
    std::string name = "pointLights[" + std::to_string(index) + "]";

    shader->setVec3(name + ".position", m_Position);

    shader->setVec3(name + ".ambient", m_Ambient);
    shader->setVec3(name + ".diffuse", m_Diffuse);
    shader->setVec3(name + ".specular", m_Specular);

    shader->setFloat(name + ".constant", m_Constant);
    shader->setFloat(name + ".linear", m_Linear);
    shader->setFloat(name + ".quad", m_Quad);

    shader->setVec3(name + ".color", m_Enabled ? m_Color : glm::vec3(0.0f));
}

void PointLight::enableBillboard()
{
    m_DrawBillboard = !m_DrawBillboard;
}

SpotLight::SpotLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 pos, glm::vec3 dir,
                     glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                     float constant, float linear, float quad,
                     float inner_cutoff, float outer_cutoff)
    : Light(enabled, name, color, ambient, diffuse, specular), m_Position(pos), m_Direction(dir),
      m_Constant(constant), m_Linear(linear), m_Quad(quad),
      m_InnerCutoff(inner_cutoff), m_OuterCutoff(outer_cutoff)
{
}

void SpotLight::updateUni(std::shared_ptr<Shader>& shader, int index) const
{
    std::string name = "spotLights[" + std::to_string(index) + "]";
    
    shader->setVec3(name + ".position", m_Position);
    shader->setVec3(name + ".direction", m_Direction);

    shader->setVec3(name + ".ambient", m_Ambient);
    shader->setVec3(name + ".diffuse", m_Diffuse);
    shader->setVec3(name + ".specular", m_Specular);

    shader->setFloat(name + ".constant", m_Constant);
    shader->setFloat(name + ".linear", m_Linear);
    shader->setFloat(name + ".quad", m_Quad);
    
    shader->setFloat(name + ".innerCutoff", glm::cos(glm::radians(m_InnerCutoff)));
    shader->setFloat(name + ".outerCutoff", glm::cos(glm::radians(m_OuterCutoff)));
    
    shader->setVec3(name + ".color", m_Enabled ? m_Color : glm::vec3(0.0f));
}

void SpotLight::enableBillboard()
{
    m_DrawBillboard = !m_DrawBillboard;
}