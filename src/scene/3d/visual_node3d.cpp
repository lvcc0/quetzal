#include "scene/3d/visual_node3d.h"

namespace qtzl
{
    VisualNode3D::VisualNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Type = Object::Type::VISUAL_NODE3D;
        this->m_Renderable = true;
    }

    void VisualNode3D::set(const std::string& property_name, const glm::vec3& property)
    {
        this->m_ModelMatrix = glm::mat4(1.0f);

        if (property_name == "Scale")
        {
            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->getVec3("Global position"));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, property * this->getVec3("Scale"));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").z, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (property_name == "Global position")
        {
            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, property + this->getVec3("Global position"));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, this->getVec3("Scale"));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").z, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (property_name == "Global rotation")
        {
            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->getVec3("Global position"));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, this->getVec3("Scale"));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (property + this->getVec3("Global rotation")).x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (property + this->getVec3("Global rotation")).y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (property + this->getVec3("Global rotation")).z, glm::vec3(0.0f, 0.0f, 1.0f));
        }

        this->m_Vec3Properties[property_name] = property;
    }

    void VisualNode3D::setScale(const glm::vec3& scale)
    {
        this->set("Scale", scale);
    }

    void VisualNode3D::setGlobalPosition(const glm::vec3& position)
    {
        this->set("Global position", position);
    }

    void VisualNode3D::setGlobalRotation(const glm::vec3& radians)
    {
        this->set("Global rotation", radians);
    }

    void VisualNode3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        this->set("Global rotation", glm::radians(degrees));
    }

    glm::mat4 VisualNode3D::getModelMatrix() const
    {
        return this->m_ModelMatrix;
    }
}