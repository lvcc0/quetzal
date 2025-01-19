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
    }

    void PointLight3D::set(const std::string& property_name, const glm::vec3& property)
    {
        this->m_Vec3Properties[property_name] = property;
        
        // Move children
        if (property_name == "Global position")
        {
            for (auto& entry : this->m_Children)
            {
                entry.second->set("Global position", property + entry.second->getVec3("Global position"));
            }
        }
    }

    void PointLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const
    {
        std::string name = "pointLights[" + std::to_string(index) + "]";

        shader_program->setVec3(name + ".position", this->getVec3("Global position"));

        shader_program->setVec3(name + ".ambient", this->getVec3("Ambient"));
        shader_program->setVec3(name + ".diffuse", this->getVec3("Diffuse"));
        shader_program->setVec3(name + ".specular", this->getVec3("Specular"));

        shader_program->setFloat(name + ".constant", this->getFloat("Constant"));
        shader_program->setFloat(name + ".linear", this->getFloat("Linear"));
        shader_program->setFloat(name + ".quadratic", this->getFloat("Quadratic"));

        shader_program->setVec3(name + ".color", this->getBool("Enabled") ? this->getVec3("Color") : glm::vec3(0.0f));
    }

    void PointLight3D::render(std::shared_ptr<ShaderProgram> shader_program)
    {
    }
}