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
        : Node(name)
    {
        this->m_Type = Object::Type::LIGHT3D;

        this->addProperty("Enabled", true);
        this->addProperty("Color", color);
        this->addProperty("Ambient", ambient);
        this->addProperty("Diffuse", diffuse);
        this->addProperty("Specular", specular);
    }
}