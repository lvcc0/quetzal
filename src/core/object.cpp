#include "core/object.h"

namespace qtzl
{
    Object::Object(Type type)
        : m_Type(type)
    {
    }

    void Object::set(const std::string& property_name, int property)
    {
        if (this->m_IntProperties.find(property_name) == this->m_IntProperties.end())
            return;

        this->m_IntProperties[property_name] = property;
    }

    void Object::set(const std::string& property_name, float property)
    {
        if (this->m_FloatProperties.find(property_name) == this->m_FloatProperties.end())
            return;

        this->m_FloatProperties[property_name] = property;
    }

    void Object::set(const std::string& property_name, bool property)
    {
        if (this->m_BoolProperties.find(property_name) == this->m_BoolProperties.end())
            return;

        this->m_BoolProperties[property_name] = property;
    }

    void Object::set(const std::string& property_name, const std::string& property)
    {
        if (this->m_StringProperties.find(property_name) == this->m_StringProperties.end())
            return;

        this->m_StringProperties[property_name] = property;
    }

    void Object::set(const std::string& property_name, const glm::vec3& property)
    {
        if (this->m_Vec3Properties.find(property_name) == this->m_Vec3Properties.end())
            return;

        this->m_Vec3Properties[property_name] = property;
    }

    int Object::getInt(const std::string& property_name) const
    {
        return this->m_IntProperties.at(property_name);
    }

    float Object::getFloat(const std::string& property_name) const
    {
        return this->m_FloatProperties.at(property_name);
    }

    bool Object::getBool(const std::string& property_name) const
    {
        return this->m_BoolProperties.at(property_name);
    }

    std::string Object::getString(const std::string& property_name) const
    {
        return this->m_StringProperties.at(property_name);
    }

    glm::vec3 Object::getVec3(const std::string& property_name) const
    {
        return this->m_Vec3Properties.at(property_name);
    }

    std::map<std::string, int> Object::getIntProperties() const
    {
        return this->m_IntProperties;
    }

    std::map<std::string, float> Object::getFloatProperties() const
    {
        return this->m_FloatProperties;
    }

    std::map<std::string, bool> Object::getBoolProperties() const
    {
        return this->m_BoolProperties;
    }

    std::map<std::string, std::string> Object::getStringProperties() const
    {
        return this->m_StringProperties;
    }

    std::map<std::string, glm::vec3> Object::getVec3Properties() const
    {
        return this->m_Vec3Properties;
    }

    Object::Type Object::getType() const
    {
        return this->m_Type;
    }

    bool Object::isType(Type type) const
    {
        return this->m_Type == type;
    }

    void Object::addProperty(const std::string& property_name, const int& property)
    {
        this->m_IntProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const float& property)
    {
        this->m_FloatProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const bool& property)
    {
        this->m_BoolProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const std::string& property)
    {
        this->m_StringProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const glm::vec3& property)
    {
        this->m_Vec3Properties.emplace(property_name, property);
    }
}