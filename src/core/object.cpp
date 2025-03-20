#include "core/object.h"

namespace qtzl
{
    Object::Object(Type type)
        : m_Type(type)
    {
    }

    glm::vec2 Object::getEditingLimits(const std::string& property_name) const
    {
        if (!this->m_EditingLimits.contains(property_name))
            return glm::vec2(0.0f);

        return this->m_EditingLimits.at(property_name);
    }

    float Object::getEditingSpeed(const std::string& property_name) const
    {
        if (!this->m_EditingSpeeds.contains(property_name))
            return 0.1f;

        return this->m_EditingSpeeds.at(property_name);
    }

    Object::Type Object::getType() const
    {
        return this->m_Type;
    }

    bool Object::isType(Type type) const
    {
        return this->m_Type == type;
    }
}