#include "scene/3d/node3d.h"

namespace qtzl
{
    Node3D::Node3D(const std::string& name)
        : Node(name)
    {
        this->m_Type = Object::Type::NODE3D;

        this->addProperty("Visible", true);
        this->addProperty("Global position", glm::vec3(0.0f));
        this->addProperty("Global rotation", glm::vec3(0.0f));
        this->addProperty("Position", glm::vec3(0.0f));
        this->addProperty("Rotation", glm::vec3(0.0f));
        this->addProperty("Scale", glm::vec3(1.0f));
    }

    void Node3D::setScale(const glm::vec3& scale)
    {
        this->set("scale", scale);
    }

    void Node3D::setPosition(const glm::vec3& position)
    {
        this->set("Position", position);
    }

    void Node3D::setRotation(const glm::vec3& radians)
    {
        this->set("Rotation", radians);
    }

    void Node3D::setRotationDegrees(const glm::vec3& degrees)
    {
        this->set("Rotation", glm::radians(degrees));
    }

    void Node3D::setGlobalPosition(const glm::vec3& position)
    {
        this->set("Global position", position);

        for (auto& entry : this->m_Children)
        {
            entry.second->set("Global position", position + entry.second->getVec3("Global position"));
        }
    }

    void Node3D::setGlobalRotation(const glm::vec3& radians)
    {
        this->set("Global rotation", radians);

        for (auto& entry : this->m_Children)
        {
            entry.second->set("Global rotation", radians + entry.second->getVec3("Global rotation"));
        }
    }

    void Node3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        this->set("Global rotation", glm::radians(degrees));

        for (auto& entry : this->m_Children)
        {
            entry.second->set("Global rotation", glm::radians(degrees) + entry.second->getVec3("Global rotation"));
        }
    }

    void Node3D::setVisible(bool visible)
    {
        if (this->getBool("Visible") == visible)
            return;

        this->set("Visible", visible);
    }

    glm::vec3 Node3D::getScale() const
    {
        return this->getVec3("Scale");
    }

    glm::vec3 Node3D::getPosition() const
    {
        return this->getVec3("Position");
    }

    glm::vec3 Node3D::getRotation() const
    {
        return this->getVec3("Rotation");
    }

    glm::vec3 Node3D::getRotationDegrees() const
    {
        return glm::degrees(this->getVec3("Rotation"));
    }

    glm::vec3 Node3D::getGlobalPosition() const
    {
        return this->getVec3("Global position");
    }

    glm::vec3 Node3D::getGlobalRotation() const
    {
        return this->getVec3("Global rotation");
    }

    glm::vec3 Node3D::getGlobalRotationDegrees() const
    {
        return glm::degrees(this->getVec3("Global rotation"));
    }

    bool Node3D::isVisible() const
    {
        return this->getBool("Visible");;
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