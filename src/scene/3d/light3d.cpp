#include "scene/3d/light3d.h"

namespace qtzl
{
    Light3D::Light3D(
        const std::string& name,
        glm::vec3 color,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular
    )
        : Node3D(name), m_Color(color), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular)
    {
        this->m_Type = Object::Type::LIGHT3D;
    }

    void Light3D::setEnabled(bool enabled)
    {
        if (this->m_Enabled == enabled)
            return;

        this->m_Enabled = enabled;
    }

    bool Light3D::isEnabled() const
    {
        return this->m_Enabled;
    }

    void Light3D::enable()
    {
        this->setEnabled(true);
    }

    void Light3D::disable()
    {
        this->setEnabled(false);
    }
}