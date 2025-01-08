#include "scene/3d/directional_light3d.h"

namespace qtzl
{
    DirectionalLight3D::DirectionalLight3D(
        const std::string& name,
        glm::vec3 direction,
        glm::vec3 color,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular
    )
        : Light3D(name, color, ambient, diffuse, specular), m_Direction(direction)
    {
    }

    void DirectionalLight3D::updateUniforms(const std::shared_ptr<Shader>& shader, int index) const
    {
        std::string name = "dirLights[" + std::to_string(index) + "]";

        shader->setVec3(name + ".direction", m_Direction);

        shader->setVec3(name + ".ambient", m_Ambient);
        shader->setVec3(name + ".diffuse", m_Diffuse);
        shader->setVec3(name + ".specular", m_Specular);

        shader->setVec3(name + ".color", m_Enabled ? m_Color : glm::vec3(0.0f));
    }
}