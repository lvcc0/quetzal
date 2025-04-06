#include "scene/3d/rigid_body3d.h"

namespace qtzl
{
    RigidBody3D::RigidBody3D(const std::string& name, std::shared_ptr<Model3D> model_sptr)
        : Node3D(name), m_Model_sptr(model_sptr)
    {
        this->m_Type = Object::Type::RIGID_BODY3D;
        m_Physics_sptr->m_PhysParentObject = std::make_shared<RigidBody3D>(*this);
    }

    void RigidBody3D::setScale(const glm::vec3& scale)
    {
        m_Model_sptr->setScale(scale);
        m_Physics_sptr->setScale(scale);
    }

    void RigidBody3D::setPosition(const glm::vec3& position)
    {
        m_Model_sptr->setPosition(position);
        m_Physics_sptr->setPosition(position);
    }

    void RigidBody3D::setRotation(const glm::vec3& radians)
    {
        m_Model_sptr->setRotation(radians);
        m_Physics_sptr->setRotation(radians);
    }

    void RigidBody3D::setRotationDegrees(const glm::vec3& degrees)
    {
        m_Model_sptr->setRotationDegrees(degrees);
        m_Physics_sptr->setRotationDegrees(degrees);
    }

    void RigidBody3D::setGlobalPosition(const glm::vec3& position)
    {
        m_Model_sptr->setGlobalPosition(position);
        m_Physics_sptr->setGlobalPosition(position);
    }

    void RigidBody3D::setGlobalRotation(const glm::vec3& radians)
    {
        m_Model_sptr->setGlobalRotation(radians);
        m_Physics_sptr->setGlobalRotation(radians);
    }

    void RigidBody3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        m_Model_sptr->setGlobalRotationDegrees(degrees);
        m_Physics_sptr->setGlobalRotationDegrees(degrees);
    }

    void RigidBody3D::scale(const glm::vec3& scale)
    {
        m_Model_sptr->scale(scale);
        m_Physics_sptr->scale(scale);
    }

    void RigidBody3D::translate(const glm::vec3& position)
    {
        m_Model_sptr->translate(position);
        m_Physics_sptr->translate(position);
    }

    void RigidBody3D::rotate(const glm::vec3& radians)
    {
        m_Model_sptr->rotate(radians);
        m_Physics_sptr->rotate(radians);
    }

    void RigidBody3D::rotateDegrees(const glm::vec3& degrees)
    {
        m_Model_sptr->rotateDegrees(degrees);
        m_Physics_sptr->rotateDegrees(degrees);
    }

    void RigidBody3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}