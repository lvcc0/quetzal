#include "scene/3d/spot_light3d.h"

namespace qtzl
{
    SpotLight3D::SpotLight3D(
        const std::string& name,
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& color,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        float inner_cutoff,
        float outer_cutoff
    )
        : Light3D(name, color, ambient, diffuse, specular),
        m_GlobalPosition(position), m_Direction(direction), m_Constant(constant),
        m_Linear(linear), m_Quadratic(quadratic), m_InnerCutoff(inner_cutoff), m_OuterCutoff(outer_cutoff)
    {
        this->m_Type = Object::Type::SPOT_LIGHT3D;
    }

    //void SpotLight3D::set(const std::string& property_name, const glm::vec3& value)
    //{
    //    if (!this->m_Vec3Properties.contains(property_name))
    //    {
    //        std::cerr << "ERROR::qtzl::SpotLight3D::set: no such property \"" << property_name << "\"." << std::endl;
    //        return;
    //    }

    //    // Move children
    //    if (property_name == "Global position")
    //    {
    //        for (auto& entry : this->m_Children)
    //        {
    //            entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
    //        }
    //    }

    //    if (this->m_EditingLimits.contains(property_name))
    //    {
    //        this->m_Vec3Properties[property_name].value.x = std::min(std::max(value.x, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
    //        this->m_Vec3Properties[property_name].value.y = std::min(std::max(value.y, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
    //        this->m_Vec3Properties[property_name].value.z = std::min(std::max(value.z, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
    //    }
    //    else
    //    {
    //        this->m_Vec3Properties[property_name].value = value;
    //    }
    //}

    void SpotLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program) const
    {
        shader_program->setVec3(m_Name + ".position", m_GlobalPosition);
        shader_program->setVec3(m_Name + ".direction", m_Direction);

        shader_program->setVec3(m_Name + ".ambient", m_Ambient);
        shader_program->setVec3(m_Name + ".diffuse", m_Diffuse);
        shader_program->setVec3(m_Name + ".specular", m_Specular);

        shader_program->setFloat(m_Name + ".constant", m_Constant);
        shader_program->setFloat(m_Name + ".linear", m_Linear);
        shader_program->setFloat(m_Name + ".quadratic", m_Quadratic);

        shader_program->setFloat(m_Name + ".innerCutoff", glm::cos(glm::radians(m_InnerCutoff)));
        shader_program->setFloat(m_Name + ".outerCutoff", glm::cos(glm::radians(m_OuterCutoff)));

        shader_program->setVec3(m_Name + ".color", m_IsEnabled ? m_Color : glm::vec3(0.0f));
    }
    void SpotLight3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}