#include "sphere_collision.h"

namespace qtzl
{
    SphereCollision::SphereCollision(const std::string& name, unsigned int streangth)
        : PhysicsNode3D(name, streangth)
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
    void SphereCollision::setScale(const glm::vec3& scale)
    {
        m_Radius = std::max(std::max(scale.x, scale.y), scale.z);
    }
}