#include "scene/3d/spot_light3d.h"

namespace qtzl
{
    SpotLight3D::SpotLight3D(
        const std::string& name,
        glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 color,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float constant,
        float linear,
        float quadratic,
        float inner_cutoff,
        float outer_cutoff
    )
        : Light3D(name, color, ambient, diffuse, specular), m_Position(position), m_Direction(direction),
          m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic),
          m_InnerCutoff(inner_cutoff), m_OuterCutoff(outer_cutoff)
    {
    }

    void SpotLight3D::setBillboardEnabled(bool enabled)
    {
        if (this->m_BillboardEnabled == enabled)
            return;

        this->m_BillboardEnabled = enabled;
    }

    void SpotLight3D::enableBillboard()
    {
        this->setBillboardEnabled(true);
    }

    void SpotLight3D::disableBillboard()
    {
        this->setBillboardEnabled(false)
    }

    void SpotLight3D::updateUniforms(const std::shared_ptr<Shader>& shader, int index) const
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
}