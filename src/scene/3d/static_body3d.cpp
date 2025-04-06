#include "scene/3d/static_body3d.h"

namespace qtzl
{
    StaticBody3D::StaticBody3D(const std::string& name, std::shared_ptr<Model3D> model_sptr)
        : Node3D(name), m_Model_sptr(model_sptr)
    {
        this->m_Type = Object::Type::STATIC_BODY3D;
        m_Physics_sptr->m_PhysParentObject = std::make_shared<StaticBody3D>(*this);
    }

    void StaticBody3D::setScale(const glm::vec3& scale)
    {
        m_Model_sptr->setScale(scale);
        m_Physics_sptr->setScale(scale);
    }

    void StaticBody3D::setPosition(const glm::vec3& position)
    {
        m_Model_sptr->setPosition(position);
        m_Physics_sptr->setPosition(position);
    }

    void StaticBody3D::setRotation(const glm::vec3& radians)
    {
        m_Model_sptr->setRotation(radians);
        m_Physics_sptr->setRotation(radians);
    }

    void StaticBody3D::setRotationDegrees(const glm::vec3& degrees)
    {
        m_Model_sptr->setRotationDegrees(degrees);
        m_Physics_sptr->setRotationDegrees(degrees);
    }

    void StaticBody3D::setGlobalPosition(const glm::vec3& position)
    {
        m_Model_sptr->setGlobalPosition(position);
        m_Physics_sptr->setGlobalPosition(position);
    }

    void StaticBody3D::setGlobalRotation(const glm::vec3& radians)
    {
        m_Model_sptr->setGlobalRotation(radians);
        m_Physics_sptr->setGlobalRotation(radians);
    }

    void StaticBody3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
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

    void StaticBody3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}