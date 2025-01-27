#include "core/object.h"

namespace qtzl
{
    Object::Object(Type type)
        : m_Type(type)
    {
    }

    void Object::set(const std::string& property_name, int value)
    {
        if (!this->m_IntProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::set (int): no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_IntProperties[property_name].value = glm::clamp(value, (int)this->m_EditingLimits.at(property_name).x, (int)this->m_EditingLimits.at(property_name).y);
            return;
        }

        this->m_IntProperties[property_name].value = value;
    }

    void Object::set(const std::string& property_name, float value)
    {
        if (!this->m_FloatProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::set (float): no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_FloatProperties[property_name].value = glm::clamp(value, this->m_EditingLimits.at(property_name).x, this->m_EditingLimits.at(property_name).y);
            return;
        }

        this->m_FloatProperties[property_name].value = value;
    }

    void Object::set(const std::string& property_name, bool value)
    {
        if (!this->m_BoolProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::set (bool): no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        this->m_BoolProperties[property_name].value = value;
    }

    void Object::set(const std::string& property_name, const std::string& value)
    {
        if (!this->m_StringProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::set (string): no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        this->m_StringProperties[property_name].value = value;
    }

    void Object::set(const std::string& property_name, const glm::vec3& value)
    {
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::set (vec3): no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_Vec3Properties[property_name].value = glm::clamp(value, this->m_EditingLimits.at(property_name).x, this->m_EditingLimits.at(property_name).y);
            return;
        }

        this->m_Vec3Properties[property_name].value = value;
    }

    int Object::getInt(const std::string& property_name) const
    {
        if (!this->m_IntProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::getInt: no such property \"" << property_name << "\"." << std::endl;
            return 0;
        }

        return this->m_IntProperties.at(property_name).value;
    }

    float Object::getFloat(const std::string& property_name) const
    {
        if (!this->m_FloatProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::getFloat: no such property \"" << property_name << "\"." << std::endl;
            return 0.0f;
        }

        return this->m_FloatProperties.at(property_name).value;
    }

    bool Object::getBool(const std::string& property_name) const
    {
        if (!this->m_BoolProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::getBool: no such property \"" << property_name << "\"." << std::endl;
            return false;
        }

        return this->m_BoolProperties.at(property_name).value;
    }

    std::string Object::getString(const std::string& property_name) const
    {
        if (!this->m_StringProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::getString: no such property \"" << property_name << "\"." << std::endl;
            return "";
        }

        return this->m_StringProperties.at(property_name).value;
    }

    glm::vec3 Object::getVec3(const std::string& property_name) const
    {
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::getVec3: no such property \"" << property_name << "\"." << std::endl;
            return glm::vec3(0.0f);
        }

        return this->m_Vec3Properties.at(property_name).value;
    }

    std::map<std::string, Object::Property<int>> Object::getIntProperties() const
    {
        return this->m_IntProperties;
    }

    std::map<std::string, Object::Property<float>> Object::getFloatProperties() const
    {
        return this->m_FloatProperties;
    }

    std::map<std::string, Object::Property<bool>> Object::getBoolProperties() const
    {
        return this->m_BoolProperties;
    }

    std::map<std::string, Object::Property<std::string>> Object::getStringProperties() const
    {
        return this->m_StringProperties;
    }

    std::map<std::string, Object::Property<glm::vec3>> Object::getVec3Properties() const
    {
        return this->m_Vec3Properties;
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

    bool Object::has(const std::string& property_name) const
    {
        return this->m_IntProperties.contains(property_name)    ||
               this->m_FloatProperties.contains(property_name)  ||
               this->m_BoolProperties.contains(property_name)   ||
               this->m_StringProperties.contains(property_name) ||
               this->m_Vec3Properties.contains(property_name);
    }

    Object::Type Object::getType() const
    {
        return this->m_Type;
    }

    bool Object::isType(Type type) const
    {
        return this->m_Type == type;
    }

    void Object::addProperty(const std::string& property_name, int value, bool editable)
    {
        Property<int> property{ value, editable };

        this->m_IntProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, float value, bool editable)
    {
        Property<float> property{ value, editable };

        this->m_FloatProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, bool value, bool editable)
    {
        Property<bool> property{ value, editable };

        this->m_BoolProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const std::string& value, bool editable)
    {
        Property<std::string> property{ value, editable };

        this->m_StringProperties.emplace(property_name, property);
    }

    void Object::addProperty(const std::string& property_name, const glm::vec3& value, bool editable)
    {
        Property<glm::vec3> property{ value, editable };

        this->m_Vec3Properties.emplace(property_name, property);
    }

    void Object::removeProperty(const std::string& property_name)
    {
        // wow such code

        if (this->m_IntProperties.contains(property_name))
        {
            this->m_IntProperties.erase(property_name);
            return;
        }

        if (this->m_FloatProperties.contains(property_name))
        {
            this->m_FloatProperties.erase(property_name);
            return;
        }

        if (this->m_BoolProperties.contains(property_name))
        {
            this->m_BoolProperties.erase(property_name);
            return;
        }

        if (this->m_StringProperties.contains(property_name))
        {
            this->m_StringProperties.erase(property_name);
            return;
        }

        if (this->m_Vec3Properties.contains(property_name))
        {
            this->m_Vec3Properties.erase(property_name);
            return;
        }
    }

    void Object::setPropertyEditingLimits(const std::string& property_name, float lower_limit, float upper_limit)
    {
        if (this->m_StringProperties.contains(property_name) || this->m_BoolProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::setPropertyLimits (float, float): you can't set limits for a string or bool property." << std::endl;
            return;
        }

        this->m_EditingLimits[property_name] = glm::vec2(lower_limit, upper_limit);
    }

    void Object::setPropertyEditingLimits(const std::string& property_name, const glm::vec2& limits)
    {
        if (this->m_StringProperties.contains(property_name) || this->m_BoolProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::setPropertyLimits (vec2): you can't set limits for a string or bool property." << std::endl;
            return;
        }

        this->m_EditingLimits[property_name] = limits;
    }

    void Object::setPropertyEditingSpeed(const std::string& property_name, float speed)
    {
        if (this->m_StringProperties.contains(property_name) || this->m_BoolProperties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Object::setPropertySpeed: you can't set speed for a string or bool property." << std::endl;
            return;
        }

        this->m_EditingSpeeds[property_name] = speed;
    }

    void Object::setPropertyEditable(const std::string& property_name, bool editable)
    {
        // wow much efficient

        if (this->m_IntProperties.contains(property_name))
        {
            this->m_IntProperties.at(property_name).editable = editable;
            return;
        }

        if (this->m_FloatProperties.contains(property_name))
        {
            this->m_FloatProperties.at(property_name).editable = editable;
            return;
        }

        if (this->m_BoolProperties.contains(property_name))
        {
            this->m_BoolProperties.at(property_name).editable = editable;
            return;
        }

        if (this->m_StringProperties.contains(property_name))
        {
            this->m_StringProperties.at(property_name).editable = editable;
            return;
        }

        if (this->m_Vec3Properties.contains(property_name))
        {
            this->m_Vec3Properties.at(property_name).editable = editable;
            return;
        }
    }
}