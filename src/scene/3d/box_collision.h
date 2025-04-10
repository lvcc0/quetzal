#pragma once

#include "scene/3d/physics_node3d.h"

// TODO: scale vs size (?)

namespace qtzl
{
    class BoxCollision : public PhysicsNode3D
    {
    public:
        BoxCollision(const std::string& name, unsigned int streangth);
        virtual ~BoxCollision() = default;

        void setSize(const glm::vec3& size);

        void accept(NodeVisitor& visitor) override;
        void setScale(const glm::vec3& scale) override;

        glm::vec3 m_Size;
    };
}