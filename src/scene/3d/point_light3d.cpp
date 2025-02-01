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
        : Light3D(name, color, ambient, diffuse, specular)
    {
        this->m_Type = Object::Type::POINT_LIGHT3D;

        this->addProperty("Global position", position);
        this->addProperty("Constant", constant);
        this->addProperty("Linear", linear);
        this->addProperty("Quadratic", quadratic);

        this->setPropertyEditingSpeed("Constant", 0.01f);
        this->setPropertyEditingSpeed("Linear", 0.01f);
        this->setPropertyEditingSpeed("Quadratic", 0.01f);
    }

    void PointLight3D::set(const std::string& property_name, const glm::vec3& value)
    {
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::PointLight3D::set: no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        // Move children
        if (property_name == "Global position")
        {
            for (auto& entry : this->m_Children)
            {
                entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
            }
        }

        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_Vec3Properties[property_name].value.x = std::min(std::max(value.x, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.y = std::min(std::max(value.y, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.z = std::min(std::max(value.z, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        }
        else
        {
            this->m_Vec3Properties[property_name].value = value;
        }
    }

    void PointLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program) const
    {
        shader_program->setVec3(m_Name + ".position", this->getVec3("Global position"));

        shader_program->setVec3(m_Name + ".ambient", this->getVec3("Ambient"));
        shader_program->setVec3(m_Name + ".diffuse", this->getVec3("Diffuse"));
        shader_program->setVec3(m_Name + ".specular", this->getVec3("Specular"));

        shader_program->setFloat(m_Name + ".constant", this->getFloat("Constant"));
        shader_program->setFloat(m_Name + ".linear", this->getFloat("Linear"));
        shader_program->setFloat(m_Name + ".quadratic", this->getFloat("Quadratic"));

        shader_program->setVec3(m_Name + ".color", this->getBool("Enabled") ? this->getVec3("Color") : glm::vec3(0.0f));
    }
    void PointLight3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}