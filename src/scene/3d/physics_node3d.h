#pragma once

#include <functional>

#include "scene/3d/rigid_body3d.h"
#include "static/resource_manager.h"

namespace qtzl
{
    // TODO
    class PhysicsNode3D : public Node3D
    {
    public:
        PhysicsNode3D(const std::string& name, const glm::vec3& pos);
        virtual ~PhysicsNode3D() = default;

        const std::shared_ptr<RigidBody3D> getVisiblePart();
        // These funcs use set function
        virtual void setGlobalPosition(const glm::vec3& position) override;
        virtual void setGlobalRotation(const glm::vec3& radians) override;
        virtual void setGlobalRotationDegrees(const glm::vec3& degrees) override;
        virtual void setScale(const glm::vec3& scale) override;

        virtual void set(const std::string& property_name, const glm::vec3& value) override;
        virtual void set(const std::string& property_name, bool value) override;

    protected:
        std::shared_ptr<RigidBody3D> m_VisiblePart;
    };
}