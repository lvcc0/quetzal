#include "core/object.h"

namespace qtzl
{
    Object::Object(Type type)
        : m_Type(type)
    {
    }

    template <typename T>
    void Object::set(const std::string& property_name, const T& value)
    {
        std::cerr << "Specify the property type" << std::endl;
    }

    template <>
    void Object::set<int>(const std::string& property_name, const int& property)
    {
        if (this->m_IntProperties.find(property_name) == this->m_IntProperties.end())
            return;

        this->m_IntProperties[property_name] = property;
    }

    template <>
    void Object::set<float>(const std::string& property_name, const float& property)
    {
        if (this->m_FloatProperties.find(property_name) == this->m_FloatProperties.end())
            return;

        this->m_FloatProperties[property_name] = property;
    }

    template <>
    void Object::set<bool>(const std::string& property_name, const bool& property)
    {
        if (this->m_BoolProperties.find(property_name) == this->m_BoolProperties.end())
            return;

        this->m_BoolProperties[property_name] = property;
    }

    template <>
    void Object::set<std::string>(const std::string& property_name, const std::string& property)
    {
        if (this->m_StringProperties.find(property_name) == this->m_StringProperties.end())
            return;

        this->m_StringProperties[property_name] = property;
    }

    template <>
    void Object::set<glm::vec3>(const std::string& property_name, const glm::vec3& property)
    {
        if (this->m_Vec3Properties.find(property_name) == this->m_Vec3Properties.end())
            return;

        this->m_Vec3Properties[property_name] = property;
    }

    template <typename T>
    T Object::get(const std::string& property_name) const
    {
        std::cerr << "Specify the property type" << std::endl;
        return;
    }

    template <>
    int Object::get<int>(const std::string& property_name) const
    {
        return this->m_IntProperties.at(property_name);
    }

    template <>
    float Object::get<float>(const std::string& property_name) const
    {
        return this->m_FloatProperties.at(property_name);
    }

    template <>
    bool Object::get<bool>(const std::string& property_name) const
    {
        return this->m_BoolProperties.at(property_name);
    }

    template <>
    std::string Object::get<std::string>(const std::string& property_name) const
    {
        return this->m_StringProperties.at(property_name);
    }

    template <>
    glm::vec3 Object::get<glm::vec3>(const std::string& property_name) const
    {
        return this->m_Vec3Properties.at(property_name);
    }

    template <typename T>
    std::map<std::string, T> Object::getProperties() const
    {
        std::cerr << "Specify the property type" << std::endl;
        return;
    }

    template <>
    std::map<std::string, int> Object::getProperties() const
    {
        return this->m_IntProperties;
    }

    template <>
    std::map<std::string, float> Object::getProperties() const
    {
        return this->m_FloatProperties;
    }

    template <>
    std::map<std::string, bool> Object::getProperties() const
    {
        return this->m_BoolProperties;
    }

    template <>
    std::map<std::string, std::string> Object::getProperties() const
    {
        return this->m_StringProperties;
    }

    template <>
    std::map<std::string, glm::vec3> Object::getProperties() const
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

    template <typename T>
    void Object::addProperty(const std::string& property_name, const T& property)
    {
        std::cerr << "Specify the property type" << std::endl;
        return;
    }

    template <>
    void Object::addProperty<int>(const std::string& property_name, const int& property)
    {
        this->m_IntProperties.emplace(property_name, property);
    }

    template <>
    void Object::addProperty<float>(const std::string& property_name, const float& property)
    {
        this->m_FloatProperties.emplace(property_name, property);
    }

    template <>
    void Object::addProperty<bool>(const std::string& property_name, const bool& property)
    {
        this->m_BoolProperties.emplace(property_name, property);
    }

    template <>
    void Object::addProperty<std::string>(const std::string& property_name, const std::string& property)
    {
        this->m_StringProperties.emplace(property_name, property);
    }

    template <>
    void Object::addProperty<glm::vec3>(const std::string& property_name, const glm::vec3& property)
    {
        this->m_Vec3Properties.emplace(property_name, property);
    }
}