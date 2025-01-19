#pragma once

#include "core/rendering/shader_program.h"

#include "scene/main/node.h"

namespace qtzl
{
    // Abstract base class for 3d lights
    class Light3D : public Node
    {
    public:
        Light3D(
            const std::string& name,
            const glm::vec3& color = glm::vec3(1.0f),
            const glm::vec3& ambient = glm::vec3(0.5f),
            const glm::vec3& diffuse = glm::vec3(0.5f),
            const glm::vec3& specular = glm::vec3(0.5f)
        );
        virtual ~Light3D() = default;

        virtual void updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const = 0;
    };
}