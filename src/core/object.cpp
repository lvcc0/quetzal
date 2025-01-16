#include "core/object.h"

namespace qtzl
{
    Object::Object(Type type)
        : m_Type(type)
    {
    }

    void Object::setProperty(const std::string& property_name, int value)
    {
        if (this->m_Properties.find(property_name) == this->m_Properties.end())
            return;

        if (this->m_Properties.at(property_name).type != Property::INT)
            return;

        this->m_Properties[property_name] = { value, Property::INT };
    }

    void Object::setProperty(const std::string& property_name, float value)
    {
        if (this->m_Properties.find(property_name) == this->m_Properties.end())
            return;

        if (this->m_Properties.at(property_name).type != Property::FLOAT)
            return;

        this->m_Properties[property_name] = { value, Property::FLOAT };
    }

    void Object::setProperty(const std::string& property_name, bool value)
    {
        if (this->m_Properties.find(property_name) == this->m_Properties.end())
            return;

        if (this->m_Properties.at(property_name).type != Property::BOOL)
            return;

        this->m_Properties[property_name] = { value, Property::BOOL };
    }

    void Object::setProperty(const std::string& property_name, const std::string& value)
    {
        if (this->m_Properties.find(property_name) == this->m_Properties.end())
            return;

        if (this->m_Properties.at(property_name).type != Property::STRING)
            return;

        this->m_Properties[property_name] = { value, Property::STRING };
    }
    
    void Object::setProperty(const std::string& property_name, const glm::vec3& value)
    {
        if (this->m_Properties.find(property_name) == this->m_Properties.end())
            return;

        if (this->m_Properties.at(property_name).type != Property::VEC3)
            return;

        this->m_Properties[property_name] = { value, Property::VEC3 };
    }

    Object::Property Object::getProperty(const std::string& property_name) const
    {
        return this->m_Properties.at(property_name);
    }

    std::map<std::string, Object::Property> Object::getProperties() const
    {
        return this->m_Properties;
    }

    Object::Type Object::getType() const
    {
        return this->m_Type;
    }

    bool Object::isType(Type type) const
    {
        return this->m_Type == type;
    }

    void Object::addProperty(const std::string& property_name, Property& property)
    {
        this->m_Properties.emplace(property_name, property);
    }

    void Object::addProperties(std::map<std::string, Property>& properties)
    {
        for (const auto& entry : properties)
        {
            this->m_Properties.emplace(entry);
        }
    }
}