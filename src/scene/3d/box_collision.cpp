#include "box_collision.h"

namespace qtzl
{
    BoxCollision::BoxCollision(const std::string& name, unsigned int streangth)
        : PhysicsNode3D(name, streangth)
    {
        this->m_Type = Object::Type::BOX_COLLISION;

        m_Size = glm::vec3(1.0f);
    }

    void BoxCollision::setSize(const glm::vec3& size)
    {
        m_Size = size;
    }
    void BoxCollision::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
    void BoxCollision::setScale(const glm::vec3& scale)
    {
        m_Size = scale;
    }
}