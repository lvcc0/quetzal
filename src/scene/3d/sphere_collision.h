#pragma once

#include "scene/3d/physics_node3d.h"

namespace qtzl
{
    class SphereCollision : public PhysicsNode3D
    {
    public:
        SphereCollision(const std::string& name);
        virtual ~SphereCollision() = default;

        void setRadius(float radius);

        void accept(NodeVisitor& visitor) override;
    };
}