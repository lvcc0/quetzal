#include "scene/3d/point_light3d.h"

namespace qtzl
{
    PointLight3D::PointLight3D(
        const std::string& name,
        const glm::vec3& position,
        const glm::vec3& color,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic
    )
        : Light3D(name, color, ambient, diffuse, specular),
        m_GlobalPosition(position), m_Constant(constant),
        m_Linear(linear), m_Quadratic(quadratic)
    {
        this->m_Type = Object::Type::POINT_LIGHT3D;
        //this->setPropertyEditingSpeed("Constant", 0.01f);
        //this->setPropertyEditingSpeed("Linear", 0.01f);
        //this->setPropertyEditingSpeed("Quadratic", 0.01f);
    }

    //void PointLight3D::set(const std::string& property_name, const glm::vec3& value)
    //{
    //    if (!this->m_Vec3Properties.contains(property_name))
    //    {
    //        std::cerr << "ERROR::qtzl::PointLight3D::set: no such property \"" << property_name << "\"." << std::endl;
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

    void PointLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program) const
    {
        shader_program->setVec3(m_Name + ".position", m_GlobalPosition);

        shader_program->setVec3(m_Name + ".ambient", m_Ambient);
        shader_program->setVec3(m_Name + ".diffuse", m_Diffuse);
        shader_program->setVec3(m_Name + ".specular", m_Specular);

        shader_program->setFloat(m_Name + ".constant", m_Constant);
        shader_program->setFloat(m_Name + ".linear", m_Linear);
        shader_program->setFloat(m_Name + ".quadratic", m_Quadratic);

        shader_program->setVec3(m_Name + ".color", m_IsEnabled ? m_Color : glm::vec3(0.0f));
    }
    void PointLight3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}