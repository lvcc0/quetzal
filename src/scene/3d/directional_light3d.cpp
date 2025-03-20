#include "scene/3d/directional_light3d.h"

namespace qtzl
{
    DirectionalLight3D::DirectionalLight3D(
        const std::string& name,
        const glm::vec3& direction,
        const glm::vec3& color,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular
    )
        : Light3D(name, color, ambient, diffuse, specular), m_Direction(direction)
    {
        this->m_Type = Object::Type::DIRECTIONAL_LIGHT3D;
    }

    void DirectionalLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program) const
    {
        shader_program->setVec3(m_Name + ".direction", m_Direction);

        shader_program->setVec3(m_Name + ".ambient", m_Ambient);
        shader_program->setVec3(m_Name + ".diffuse", m_Diffuse);
        shader_program->setVec3(m_Name + ".specular", m_Specular);

        shader_program->setVec3(m_Name + ".color", m_IsEnabled ? m_Color : glm::vec3(0.0f));
    }
    void DirectionalLight3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}