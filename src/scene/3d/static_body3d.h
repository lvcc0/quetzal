#pragma once

#include "scene/3d/model3d.h"
#include "scene/3d/sphere_collision.h"
#include "scene/3d/box_collision.h"

#include "scene/resources/mesh.h"

class PhysicsVisitor;

namespace qtzl
{
    // 3D body that can't be affected by external forces (but can affect other objects)
    class StaticBody3D : public Node3D
    {
        friend PhysicsVisitor;
    public:
        StaticBody3D(const std::string& name, std::shared_ptr<Model3D> model_sptr);
        virtual ~StaticBody3D() = default;

        void setScale(const glm::vec3& scale) override;

        void setPosition(const glm::vec3& position) override;
        void setRotation(const glm::vec3& radians) override;
        void setRotationDegrees(const glm::vec3& degrees) override;

        void setGlobalPosition(const glm::vec3& position) override;
        void setGlobalRotation(const glm::vec3& radians) override;
        void setGlobalRotationDegrees(const glm::vec3& degrees) override;

        void scale(const glm::vec3& scale) override;
        void translate(const glm::vec3& position) override;
        void rotate(const glm::vec3& radians) override;
        void rotateDegrees(const glm::vec3& degrees) override;

    private:
        std::shared_ptr<Model3D> m_Model_sptr;
        std::shared_ptr<qtzl::PhysicsNode3D> m_Physics_sptr;

        void accept(NodeVisitor& visitor) override;
    };
}