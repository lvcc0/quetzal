#include "scene/3d/node3d.h"

namespace qtzl
{
    Node3D::Node3D(const std::string& name)
        : Node(name)
    {
        this->m_Type = Object::Type::NODE3D;
        m_IsVisible = true;
        m_GlobalPosition = glm::vec3(0.0f);
        m_GlobalRotation = glm::vec3(0.0f);
        m_Position = glm::vec3(0.0f);
        m_Rotation = glm::vec3(0.0f);
        m_Scale = glm::vec3(1.0f);
    }

    //void Node3D::set(const std::string& property_name, const glm::vec3& value)
    //{
    //    if (!this->m_Vec3Properties.contains(property_name))
    //    {
    //        std::cerr << "ERROR::qtzl::Node3D::set (vec3): no such property \"" << property_name << "\"." << std::endl;
    //        return;
    //    }

    //    // Change some children properties
    //    if (property_name == "Global position")
    //    {
    //        for (auto& entry : this->m_Children)
    //        {
    //            entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
    //        }
    //    }
    //    else if (property_name == "Global rotation")
    //    {
    //        for (auto& entry : this->m_Children)
    //        {
    //            entry.second->set("Global rotation", value + entry.second->getVec3("Global rotation") - this->getVec3("Global rotation"));
    //        }
    //    }

    //    if (this->m_EditingLimits.contains(property_name))
    //    {
    //        this->m_Vec3Properties[property_name].value = glm::clamp(value, this->m_EditingLimits.at(property_name).x, this->m_EditingLimits.at(property_name).y);
    //    }
    //    else
    //    {
    //        this->m_Vec3Properties[property_name].value = value;
    //    }
    //}

    // TODO: MAKE LIMITS
    void Node3D::setScale(const glm::vec3& scale)
    {
        m_Scale = scale;
    }

    void Node3D::setPosition(const glm::vec3& position)
    {
        m_Position = position;
    }

    void Node3D::setRotation(const glm::vec3& radians)
    {
        m_Rotation = radians;
    }

    void Node3D::setRotationDegrees(const glm::vec3& degrees)
    {
        m_Rotation = glm::radians(degrees);
    }

    void Node3D::setGlobalPosition(const glm::vec3& position)
    {
        m_GlobalPosition = position;
    }

    void Node3D::setGlobalRotation(const glm::vec3& radians)
    {
        m_GlobalRotation = radians;
    }

    void Node3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        m_GlobalRotation = glm::radians(degrees);
    }

    glm::vec3 Node3D::getScale() const
    {
        return m_Scale;
    }

    glm::vec3 Node3D::getPosition() const
    {
        return m_Position;
    }

    glm::vec3 Node3D::getRotation() const
    {
        return m_Rotation;
    }

    glm::vec3 Node3D::getRotationDegrees() const
    {
        return glm::degrees(m_Rotation);
    }

    glm::vec3 Node3D::getGlobalPosition() const
    {
        return m_GlobalPosition;
    }

    glm::vec3 Node3D::getGlobalRotation() const
    {
        return m_GlobalRotation;
    }

    glm::vec3 Node3D::getGlobalRotationDegrees() const
    {
        return glm::degrees(m_GlobalRotation);
    }

    void Node3D::scale(const glm::vec3& scale)
    {
        m_Scale += scale;
    }

    void Node3D::translate(const glm::vec3& position)
    {
        m_GlobalPosition += position;
    }

    void Node3D::rotate(const glm::vec3& radians)
    {
        m_GlobalRotation += radians;
    }

    void Node3D::rotateDegrees(const glm::vec3& degrees)
    {
        m_GlobalRotation += glm::radians(degrees);
    }

    bool Node3D::isVisible() const
    {
        return m_IsVisible;
    }

    void Node3D::show()
    {
        m_IsVisible = true;
    }

    void Node3D::hide()
    {
        m_IsVisible = false;
    }
    void Node3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}