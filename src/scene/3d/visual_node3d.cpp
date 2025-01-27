#include "scene/3d/visual_node3d.h"

namespace qtzl
{
    VisualNode3D::VisualNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Type = Object::Type::VISUAL_NODE3D;
    }

    void VisualNode3D::set(const std::string& property_name, const glm::vec3& value)
    {
        Node3D::set(property_name, value);

        this->m_ModelMatrix = glm::mat4(1.0f);

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->getVec3("Global position"));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, this->getVec3("Scale"));
    }

    glm::mat4 VisualNode3D::getModelMatrix() const
    {
        return this->m_ModelMatrix;
    }
}