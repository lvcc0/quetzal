#include "scene/3d/static_body3d.h"

namespace qtzl
{
    // You have to set parent to phys_ptr via shared ptr with ref to this object, you have to do it in scene
    StaticBody3D::StaticBody3D(const std::string& name, glm::vec3 pos, std::shared_ptr<Model3D> model_sptr, std::shared_ptr<PhysicsNode3D> phys_sptr)
        : Node3D(name), m_Model_sptr(model_sptr)
    {
        m_GlobalPosition = pos;
        this->m_Type = Object::Type::STATIC_BODY3D;
        m_Physics_sptr = phys_sptr;
    }

    void StaticBody3D::setScale(const glm::vec3& scale)
    {
        m_Scale = scale;
        m_Model_sptr->setScale(scale);
        m_Physics_sptr->setScale(scale);
    }

    void StaticBody3D::setPosition(const glm::vec3& position)
    {
        m_Position = position;
        m_Model_sptr->setPosition(position);
        m_Physics_sptr->setPosition(position);
    }

    void StaticBody3D::setRotation(const glm::vec3& radians)
    {
        m_Rotation = radians;
        m_Model_sptr->setRotation(radians);
        m_Physics_sptr->setRotation(radians);
    }

    void StaticBody3D::setRotationDegrees(const glm::vec3& degrees)
    {
        m_Rotation = glm::radians(degrees);
        m_Model_sptr->setRotationDegrees(degrees);
        m_Physics_sptr->setRotationDegrees(degrees);
    }

    void StaticBody3D::setGlobalPosition(const glm::vec3& position)
    {
        m_GlobalPosition = position;
        m_Model_sptr->setGlobalPosition(position);
        m_Physics_sptr->setGlobalPosition(position);
    }

    void StaticBody3D::setGlobalRotation(const glm::vec3& radians)
    {
        m_GlobalRotation = radians;
        m_Model_sptr->setGlobalRotation(radians);
        m_Physics_sptr->setGlobalRotation(radians);
    }

    void StaticBody3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        m_GlobalRotation = glm::radians(degrees);
        m_Model_sptr->setGlobalRotationDegrees(degrees);
        m_Physics_sptr->setGlobalRotationDegrees(degrees);
    }

    void StaticBody3D::scale(const glm::vec3& scale)
    {
    }

    void StaticBody3D::translate(const glm::vec3& position)
    {
    }

    void StaticBody3D::rotate(const glm::vec3& radians)
    {
    }

    void StaticBody3D::rotateDegrees(const glm::vec3& degrees)
    {
    }

    void StaticBody3D::updateDependentParts()
    {
        setScale(m_Scale);
        setGlobalPosition(m_GlobalPosition);
        setGlobalRotation(m_GlobalRotation);
    }

    void StaticBody3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}