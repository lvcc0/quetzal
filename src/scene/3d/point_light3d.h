#pragma once

#include "scene/3d/light3d.h"

namespace qtzl
{
    // A light source with a given position that illuminates in all directions
    class PointLight3D : public Light3D
    {
    public:
        PointLight3D(
            const std::string& name,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& color = glm::vec3(1.0f),
            const glm::vec3& ambient = glm::vec3(0.5f),
            const glm::vec3& diffuse = glm::vec3(0.5f),
            const glm::vec3& specular = glm::vec3(0.5f),
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f
        );
        virtual ~PointLight3D() = default;

        void set(const std::string& property_name, const glm::vec3& value) override;

        void updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const override;
        void render(std::shared_ptr<ShaderProgram> shader_program) override;
    };
}