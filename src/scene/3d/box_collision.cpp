#include "box_collision.h"

namespace qtzl
{
    BoxCollision::BoxCollision(const std::string& name)
        : PhysicsNode3D(name)
    {
        this->m_Type = Object::Type::BOX_COLLISION;

        addProperty("Size", glm::vec3(1.0f));

        m_VisiblePart = std::make_shared<RigidBody3D>(name + "_visible", ResourceManager::getMesh("objects/catcube/catcube.obj"));
    }

    void BoxCollision::setSize(const glm::vec3& size)
    {
        this->set("Size", size);
    }
}