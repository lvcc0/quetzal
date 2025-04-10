#include "scene/3d/visual_node3d.h"

namespace qtzl
{
    VisualNode3D::VisualNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Type = Object::Type::VISUAL_NODE3D;
    }

    void VisualNode3D::setScale(const glm::vec3& scale)
    {
        m_Scale = scale;
        updateMatrix();
    }

    void VisualNode3D::setPosition(const glm::vec3& position)
    {
        m_Position = position;
        updateMatrix();
    }

    void VisualNode3D::setRotation(const glm::vec3& radians)
    {
        m_Rotation = glm::degrees(radians);
        updateMatrix();
    }

    void VisualNode3D::setRotationDegrees(const glm::vec3& degrees)
    {
        m_Rotation = degrees;
        updateMatrix();
    }

    void VisualNode3D::setGlobalPosition(const glm::vec3& position)
    {
        m_GlobalPosition = position;
        updateMatrix();
    }

    void VisualNode3D::setGlobalRotation(const glm::vec3& radians)
    {
        m_GlobalRotation = glm::degrees(radians);
        updateMatrix();
    }

    void VisualNode3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        m_GlobalRotation = degrees;
        updateMatrix();
    }

    void VisualNode3D::scale(const glm::vec3& scale)
    {
        m_Scale += scale;
        updateMatrix();
    }

    void VisualNode3D::translate(const glm::vec3& position)
    {
        m_GlobalPosition += position;
        updateMatrix();
    }

    void VisualNode3D::rotate(const glm::vec3& radians)
    {
        m_GlobalRotation += radians;
        updateMatrix();
    }

    void VisualNode3D::rotateDegrees(const glm::vec3& degrees)
    {
        m_GlobalRotation += glm::radians(degrees);
        updateMatrix();
    }

    glm::mat4 VisualNode3D::getModelMatrix() const
    {
        return this->m_ModelMatrix;
    }
    void VisualNode3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }

    void VisualNode3D::updateMatrix()
    {
        this->m_ModelMatrix = glm::mat4(1.0f);

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, m_GlobalPosition);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, glm::radians(m_GlobalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, glm::radians(m_GlobalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, glm::radians(m_GlobalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, m_Scale);
    }
}