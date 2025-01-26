#include "sphere_collision.h"

namespace qtzl
{
    SphereCollision::SphereCollision(const std::string& name)
        : PhysicsNode3D(name)
    {
        this->m_Type = Object::Type::SPHERE_COLLISION;

        addProperty("Radius", 1.0f);

        m_VisiblePart = std::make_shared<RigidBody3D>(name + "_visible", ResourceManager::getMesh("objects/catsphere/catsphere.obj"));
    }

    void SphereCollision::setRadius(float radius)
    {
        this->set("Radius", radius);
    }
}