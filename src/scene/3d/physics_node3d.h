#pragma once

#include "scene/3d/rigid_body3d.h"
#include "static/resource_manager.h"

// TODO: make visible part just a child ?
// NOTE: currently we can set "Size" or "Radius" to visual part of a node, this ain't good

namespace qtzl
{
    // TODO
    class PhysicsNode3D : public Node3D
    {
    public:
        PhysicsNode3D(const std::string& name);
        virtual ~PhysicsNode3D() = default;

        void set(const std::string& property_name, const glm::vec3& value) override;
        void set(const std::string& property_name, bool value) override;

        void setScale(const glm::vec3& scale) override;

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& radians);
        void setRotationDegrees(const glm::vec3& degrees);

        void setGlobalPosition(const glm::vec3& position) override;
        void setGlobalRotation(const glm::vec3& radians) override;
        void setGlobalRotationDegrees(const glm::vec3& degrees) override;

        const std::shared_ptr<RigidBody3D> getVisiblePart();

    protected:
        // TODO: make it not RigidBody3D but just VisualNode3D (?)
        std::shared_ptr<RigidBody3D> m_VisiblePart;
    };
}