#pragma once

#include "scene/3d/rigid_body3d.h"
#include "static/resource_manager.h"

namespace qtzl
{
    // Base class for every 3D node that can be affected by physics
    class PhysicsNode3D : public Node3D
    {
    public:
        PhysicsNode3D(const std::string& name);
        virtual ~PhysicsNode3D() = default;

        std::shared_ptr<Node3D> m_PhysParentObject = nullptr; // Its necessary for physics_visitor(yeah, it must be Node3D)

        void accept(NodeVisitor& visitor) override;
    };
}