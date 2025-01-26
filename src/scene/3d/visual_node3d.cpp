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
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::VisualNode3D::set: no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        // Move children
        if (property_name == "Global position")
        {
            for (auto& entry : this->m_Children)
            {
                entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
            }
        }
        else if(property_name == "Global rotation")
        {
            for (auto& entry : this->m_Children)
            {
                entry.second->set("Global rotation", value + entry.second->getVec3("Global rotation") - this->getVec3("Global rotation"));
            }
        }

        // translate -> rotate -> scale

        if (property_name == "Global position")
        {
            this->m_ModelMatrix = glm::mat4(1.0f);

            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, value);
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").z, glm::vec3(0.0f, 0.0f, 1.0f));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, this->getVec3("Scale"));
        }
        else if (property_name == "Global rotation")
        {
            this->m_ModelMatrix = glm::mat4(1.0f);

            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->getVec3("Global position"));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (value).x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (value).y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, (value).z, glm::vec3(0.0f, 0.0f, 1.0f));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, this->getVec3("Scale"));
        }
        else if (property_name == "Scale")
        {
            this->m_ModelMatrix = glm::mat4(1.0f);

            this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->getVec3("Global position"));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").x, glm::vec3(1.0f, 0.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").y, glm::vec3(0.0f, 1.0f, 0.0f));
            this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->getVec3("Global rotation").z, glm::vec3(0.0f, 0.0f, 1.0f));
            this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, value);
        }
        
        // NOTE: yeah im not sure if this will work as intended
        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_Vec3Properties[property_name].value.x = std::min(std::max(value.x, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.y = std::min(std::max(value.y, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.z = std::min(std::max(value.z, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        }
        else
        {
            this->m_Vec3Properties[property_name].value = value;
        }
    }

    glm::mat4 VisualNode3D::getModelMatrix() const
    {
        return this->m_ModelMatrix;
    }
}