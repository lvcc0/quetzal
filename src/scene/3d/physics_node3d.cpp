#include "scene/3d/physics_node3d.h"

namespace qtzl
{
    PhysicsNode3D::PhysicsNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Type = Object::Type::PHYSICS_NODE3D;
    }
    void PhysicsNode3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}