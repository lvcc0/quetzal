#include "scene/3d/light3d.h"

namespace qtzl
{
    Light3D::Light3D(
        const std::string& name,
        const glm::vec3& color,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular
    )
        : Node(name), m_Color(color), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_IsEnabled(true)
    {
        this->m_Type = Object::Type::LIGHT3D;

        //this->setPropertyEditingLimits("Color", 0.0f, 1.0f);
        //this->setPropertyEditingSpeed("Color", 0.01f);

        //this->setPropertyEditingLimits("Ambient", 0.0f, 1.0f);
        //this->setPropertyEditingSpeed("Ambient", 0.01f);

        //this->setPropertyEditingLimits("Diffuse", 0.0f, 1.0f);
        //this->setPropertyEditingSpeed("Diffuse", 0.01f);

        //this->setPropertyEditingLimits("Specular", 0.0f, 1.0f);
        //this->setPropertyEditingSpeed("Specular", 0.01f);
    }
    void Light3D::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}