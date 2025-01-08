#include "scene/3d/node3d.h"

namespace qtzl
{
    Node3D::Node3D(const std::string& name)
        : Node(name)
    {
    }

    void Node3D::setPosition(const glm::vec3& position)
    {
        this->m_Position = position;
    }

    void Node3D::setRotationDegrees(const glm::vec3& degrees)
    {
        this->m_RotationDegrees = degrees;
    }

    void Node3D::setScale(const glm::vec3& scale)
    {
        this->m_Scale = scale;
    }

    void Node3D::setGlobalPosition(const glm::vec3& position)
    {
        this->m_GlobalPosition = position;
    }

    void Node3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        this->m_GlobalRotationDegrees = degrees;
    }

    void Node3D::setVisible(bool visible)
    {
        if (this->m_Visible == visible)
            return;

        this->m_Visible = visible;
    }

    glm::vec3 Node3D::getPosition() const
    {
        return this->m_Position;
    }

    glm::vec3 Node3D::getRotationDegrees() const
    {
        return this->m_RotationDegrees;
    }

    glm::vec3 Node3D::getScale() const
    {
        return this->m_Scale;
    }

    glm::vec3 Node3D::getGlobalPosition() const
    {
        return this->m_GlobalPosition;
    }

    glm::vec3 Node3D::getGlobalRotationDegrees() const
    {
        return this->m_GlobalRotationDegrees;
    }

    bool Node3D::isVisible() const
    {
        return this->m_Visible;
    }

    void Node3D::show()
    {
        this->setVisible(true);
    }

    void Node3D::hide()
    {
        this->setVisible(false);
    }
}