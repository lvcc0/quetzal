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
        : Light3D(name, color, ambient, diffuse, specular)
    {
        this->m_Type = Object::Type::SPOT_LIGHT3D;

        this->addProperty("Global position", position);
        this->addProperty("Direction", direction);

        this->addProperty("Constant", constant);
        this->addProperty("Linear", linear);
        this->addProperty("Quadratic", quadratic);

        this->addProperty("Inner cutoff", inner_cutoff);
        this->addProperty("Outer cutoff", outer_cutoff);
    }

    void SpotLight3D::set(const std::string& property_name, const glm::vec3& value)
    {
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::SpotLight3D::set: no such property \"" << property_name << "\"." << std::endl;
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

    void SpotLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const
    {
        std::string name = "spotLights[" + std::to_string(index) + "]";

        shader_program->setVec3(name + ".position", this->getVec3("Global position"));
        shader_program->setVec3(name + ".direction", this->getVec3("Direction"));

        shader_program->setVec3(name + ".ambient", this->getVec3("Ambient"));
        shader_program->setVec3(name + ".diffuse", this->getVec3("Diffuse"));
        shader_program->setVec3(name + ".specular", this->getVec3("Specular"));

        shader_program->setFloat(name + ".constant", this->getFloat("Constant"));
        shader_program->setFloat(name + ".linear", this->getFloat("Linear"));
        shader_program->setFloat(name + ".quadratic", this->getFloat("Quadratic"));

        shader_program->setFloat(name + ".innerCutoff", glm::cos(glm::radians(this->getFloat("Inner cutoff"))));
        shader_program->setFloat(name + ".outerCutoff", glm::cos(glm::radians(this->getFloat("Outer cutoff"))));

        shader_program->setVec3(name + ".color", this->getBool("Enabled") ? this->getVec3("Color") : glm::vec3(0.0f));
    }
}