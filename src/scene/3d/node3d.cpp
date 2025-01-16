#include "scene/3d/node3d.h"

namespace qtzl
{
    Node3D::Node3D(const std::string& name)
        : Node(name)
    {
        this->m_Type = Object::Type::NODE3D;
    }

    void Node3D::setScale(const glm::vec3& scale)
    {
        this->m_Scale = scale;
    }

    void Node3D::setPosition(const glm::vec3& position)
    {
        this->m_Position = position;
    }

    void Node3D::setRotation(const glm::vec3& radians)
    {
        this->m_Rotation = radians;
    }

    void Node3D::setRotationDegrees(const glm::vec3& degrees)
    {
        this->m_Rotation = glm::radians(degrees);
    }

    void Node3D::setGlobalPosition(const glm::vec3& position)
    {
        this->m_GlobalPosition = position;

        for (auto& entry : this->m_Children)
        {
            if (std::shared_ptr<qtzl::Node3D> casted_sptr = std::dynamic_pointer_cast<qtzl::Node3D>(entry.second))
                casted_sptr->setGlobalPosition(position + casted_sptr->getPosition());
        }
    }

    void Node3D::setGlobalRotation(const glm::vec3& radians)
    {
        this->m_GlobalRotation = radians;

        for (auto& entry : this->m_Children)
        {
            if (std::shared_ptr<qtzl::Node3D> casted_sptr = std::dynamic_pointer_cast<qtzl::Node3D>(entry.second))
                casted_sptr->setGlobalPosition(radians + casted_sptr->getRotation());
        }
    }

    void Node3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        this->m_GlobalRotation = glm::radians(degrees);

        for (auto& entry : this->m_Children)
        {
            if (std::shared_ptr<qtzl::Node3D> casted_sptr = std::dynamic_pointer_cast<qtzl::Node3D>(entry.second))
                casted_sptr->setGlobalPosition(glm::radians(degrees) + casted_sptr->getRotation());
        }
    }

    void Node3D::setVisible(bool visible)
    {
        if (this->m_Visible == visible)
            return;

        this->m_Visible = visible;
    }

    glm::vec3 Node3D::getScale() const
    {
        return this->m_Scale;
    }

    glm::vec3 Node3D::getPosition() const
    {
        return this->m_Position;
    }

    glm::vec3 Node3D::getRotation() const
    {
        return this->m_Rotation;
    }

    glm::vec3 Node3D::getRotationDegrees() const
    {
        return glm::degrees(this->m_Rotation);
    }

    glm::vec3 Node3D::getGlobalPosition() const
    {
        return this->m_GlobalPosition;
    }

    glm::vec3 Node3D::getGlobalRotation() const
    {
        return this->m_GlobalRotation;
    }

    glm::vec3 Node3D::getGlobalRotationDegrees() const
    {
        return glm::degrees(this->m_GlobalRotation);
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