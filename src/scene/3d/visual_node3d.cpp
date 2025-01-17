#include "scene/3d/visual_node3d.h"

namespace qtzl
{
    VisualNode3D::VisualNode3D(const std::string& name)
        : Node3D(name)
    {
        this->m_Type = Object::Type::VISUAL_NODE3D;

        this->m_Renderable = true;
    }

    void VisualNode3D::setScale(const glm::vec3& scale)
    {
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, scale / this->get<glm::vec3>("Scale")); // TODO: fix this stuff
        this->set("Scale", scale);
    }

    void VisualNode3D::setGlobalPosition(const glm::vec3& position)
    {
        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, position - this->get<glm::vec3>("Global position"));
        this->set("Global position", position);
    }

    void VisualNode3D::setGlobalRotation(const glm::vec3& radians)
    {
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->set("Global rotation", radians);
    }

    void VisualNode3D::setGlobalRotationDegrees(const glm::vec3& degrees)
    {
        glm::vec3 radians = glm::radians(degrees);

        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (radians - this->get<glm::vec3>("Global rotation")).z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->set("Global rotation", radians);
    }
}