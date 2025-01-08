#include "scene/3d/point_light3d.h"

namespace qtzl
{
    PointLight3D::PointLight3D(
        const std::string& name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f
    )
        : Light3D(name, color, ambient, diffuse, specular), m_Position(position),
          m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic)
    {
    }

    void PointLight3D::setBillboardEnabled(bool enabled)
    {
        if (this->m_BillboardEnabled == enabled)
            return;

        this->m_BillboardEnabled = enabled;
    }

    void PointLight3D::enableBillboard()
    {
        this->setBillboardEnabled(true);
    }

    void PointLight3D::disableBillboard()
    {
        this->setBillboardEnabled(false)
    }

    void PointLight3D::updateUniforms(const std::shared_ptr<Shader>& shader, int index) const
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
}