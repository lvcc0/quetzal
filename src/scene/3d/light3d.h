#pragma once

#include "core/rendering/shader_program.h"

#include "scene/3d/node3d.h"
#include "scene/resources/shader.h"

namespace qtzl
{
    // Abstract base class for 3d lights
    class Light3D : public Node3D
    {
    public:
        Light3D
        (
            const std::string& name,
            glm::vec3 color = glm::vec3(1.0f),
            glm::vec3 ambient = glm::vec3(0.5f),
            glm::vec3 diffuse = glm::vec3(0.5f),
            glm::vec3 specular = glm::vec3(0.5f)
        );
        virtual ~Light3D() = default;

        void setEnabled(bool enabled);

        bool isEnabled() const;

        void enable();
        void disable();

        virtual void updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const = 0;

    protected:
        bool m_Enabled = true;
        
        glm::vec3 m_Color;
        
        glm::vec3 m_Ambient;
        glm::vec3 m_Diffuse;
        glm::vec3 m_Specular;
    };
}