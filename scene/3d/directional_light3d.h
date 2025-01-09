#pragma once

#include "scene/3d/light3d.h"

namespace qtzl
{
    // Directional light shining from a distance like the Sun
    class DirectionalLight3D : public Light3D
    {
    public:
        DirectionalLight3D
        (
            const std::string& name,
            glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3 color = glm::vec3(1.0f),
            glm::vec3 ambient = glm::vec3(0.5f),
            glm::vec3 diffuse = glm::vec3(0.5f),
            glm::vec3 specular = glm::vec3(0.5f)
        );
        virtual ~DirectionalLight3D() = default;

        void updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const override;

    private:
        glm::vec3 m_Direction;
    };
}