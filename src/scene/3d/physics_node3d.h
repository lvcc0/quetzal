#pragma once

#include "scene/3d/node3d.h"
#include "static/resource_manager.h"

enum CollisionType
{
    BOX, SPHERE
};

// NOTE: MAKE SCALING SIZE
namespace qtzl
{
    // Base class for every 3D node that can be affected by physics
    class PhysicsNode3D : public Node3D
    {
    public:
        PhysicsNode3D(const std::string& name, unsigned int streangth);
        virtual ~PhysicsNode3D() = default;

        inline std::shared_ptr<Node3D> getPhysParent() { return m_PhysParentObject.lock(); }
        inline void setPhysParent(std::shared_ptr<Node3D> obj) { m_PhysParentObject = obj; }

        unsigned int m_Streangth;

        void accept(NodeVisitor& visitor) override;
    private:
        std::weak_ptr<Node3D> m_PhysParentObject; // Its necessary for physics_visitor(yeah, it must be Node3D)
    };
}