#include "scene/3d/point_light3d.h"

namespace qtzl
{
    PointLight3D::PointLight3D
    (
        const std::string& name,
        glm::vec3 position,
        glm::vec3 color,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float constant,
        float linear,
        float quadratic
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
        this->setBillboardEnabled(false);
    }

    void PointLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const
    {
        std::string name = "pointLights[" + std::to_string(index) + "]";

        shader_program->setVec3(name + ".position", m_Position);

        shader_program->setVec3(name + ".ambient", m_Ambient);
        shader_program->setVec3(name + ".diffuse", m_Diffuse);
        shader_program->setVec3(name + ".specular", m_Specular);

        shader_program->setFloat(name + ".constant", m_Constant);
        shader_program->setFloat(name + ".linear", m_Linear);
        shader_program->setFloat(name + ".quadratic", m_Quadratic);

        shader_program->setVec3(name + ".color", m_Enabled ? m_Color : glm::vec3(0.0f));
    }
}