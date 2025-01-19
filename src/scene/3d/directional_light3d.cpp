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
        : Light3D(name, color, ambient, diffuse, specular)
    {
        this->m_Type = Object::Type::DIRECTIONAL_LIGHT3D;

        this->addProperty("Direction", direction);
    }

    void DirectionalLight3D::updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const
    {
        std::string name = "dirLights[" + std::to_string(index) + "]";

        shader_program->setVec3(name + ".direction", this->getVec3("Direction"));

        shader_program->setVec3(name + ".ambient", this->getVec3("Ambient"));
        shader_program->setVec3(name + ".diffuse", this->getVec3("Diffuse"));
        shader_program->setVec3(name + ".specular", this->getVec3("Specular"));

        shader_program->setVec3(name + ".color", this->getBool("Enabled") ? this->getVec3("Color") : glm::vec3(0.0f));
    }

    void DirectionalLight3D::render(std::shared_ptr<ShaderProgram> shader_program)
    {
    }
}