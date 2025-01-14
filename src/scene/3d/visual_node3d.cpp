#include "scene/3d/visual_node3d.h"

namespace qtzl
{
    VisualNode3D::VisualNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Renderable = true;
    }

    void VisualNode3D::setGlobalPosition(const glm::vec3& position)
    {
        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, position - m_GlobalPosition);
        this->m_GlobalPosition = position;
    }

    void VisualNode3D::setGlobalRotation(const glm::vec3& radians)
    {
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->m_GlobalRotation = radians;
    }

    void VisualNode3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        glm::vec3 radians = glm::radians(degrees);

        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - m_GlobalRotation).z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->m_GlobalRotation = radians;
    }
}