#pragma once

#include "scene/3d/rigid_body3d.h"
#include "static/resource_manager.h"

// NOTE: currently we can set "Size" or "Radius" to the child of a node, this ain't good

namespace qtzl
{
    // Base class for every 3D node that can be affected by physics
    class PhysicsNode3D : public Node3D
    {
    public:
        PhysicsNode3D(const std::string& name);
        virtual ~PhysicsNode3D() = default;

        void accept(NodeVisitor& visitor) override;
    };
}