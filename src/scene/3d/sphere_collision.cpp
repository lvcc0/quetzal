#include "sphere_collision.h"

namespace qtzl
{
    SphereCollision::SphereCollision(const std::string& name)
        : PhysicsNode3D(name)
    {
        this->m_Type = Object::Type::SPHERE_COLLISION;

        m_Radius = 1.0f;
    }

    void SphereCollision::setRadius(float radius)
    {
        m_Radius = radius;
    }
    void SphereCollision::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}