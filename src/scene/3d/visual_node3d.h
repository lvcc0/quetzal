#pragma once

#include "core/rendering/vertex_array.h"

#include "scene/3d/node3d.h"

namespace qtzl
{
    // Base class for every 3D node that can be drawn on the scene
    class VisualNode3D : public Node3D
    {
    public:
        VisualNode3D(const std::string& name);
        virtual ~VisualNode3D() = default;

        void setGlobalPosition(const glm::vec3& position) override;
        void setGlobalRotation(const glm::vec3& radians) override;
        void setGlobalRotationDegrees(const glm::vec3& degrees) override;

    protected:
        Variant::Type m_Type = Variant::Type::VISUAL_NODE3D;

        glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

        virtual void setupRender() = 0;
    };
}