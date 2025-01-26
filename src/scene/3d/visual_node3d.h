#pragma once

#include "core/rendering/vertex_array.h"

#include "scene/3d/node3d.h"

// TODO: rotating children

namespace qtzl
{
    // Base class for every 3D node that can be drawn on the scene
    class VisualNode3D : public Node3D
    {
    public:
        VisualNode3D(const std::string& name);
        virtual ~VisualNode3D() = default;

        virtual void set(const std::string& property_name, const glm::vec3& value) override;

        virtual glm::mat4 getModelMatrix() const;

        virtual void render(const std::shared_ptr<ShaderProgram>& shader_program) = 0;

    protected:
        glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

        virtual void setupRender() = 0;
    };
}