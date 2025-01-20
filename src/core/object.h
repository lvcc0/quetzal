#pragma once

// std
#include <iostream>
#include <map>
#include <string>

// thirdparty
#include <glm/glm.hpp>

namespace qtzl
{
    // Base class for almost everything in the engine
    class Object
    {
    private:
        template <typename T>
        struct Property
        {
            T value;
            bool editable;
        };

    public:
        enum Type
        {
            NIL,

            // main
            NODE,
            RESOURCE,

            // resources
            MESH,
            SHADER,
            TEXTURE,

            // 3d
            NODE3D,
                VISUAL_NODE3D,
                    BILLBOARD,
                        CYLINDRICAL_BILLBOARD,
                        SPHERICAL_BILLBOARD,
                    RIGID_BODY3D,
                    STATIC_BODY3D,
            LIGHT3D,
                DIRECTIONAL_LIGHT3D,
                POINT_LIGHT3D,
                SPOT_LIGHT3D,
            SKYBOX
        };

        Object(Type type);
        virtual ~Object() = default;

        // Set property if it exists

        virtual void set(const std::string& property_name, int value);
        virtual void set(const std::string& property_name, float value);
        virtual void set(const std::string& property_name, bool value);
        virtual void set(const std::string& property_name, const std::string& value);
        virtual void set(const std::string& property_name, const glm::vec3& value);

        // Get property

        int         getInt(const std::string& property_name) const;
        float       getFloat(const std::string& property_name) const;
        bool        getBool(const std::string& property_name) const;
        std::string getString(const std::string& property_name) const;
        glm::vec3   getVec3(const std::string& property_name) const;
        
        // Get all properties of type

        std::map<std::string, Property<int>>         getIntProperties() const;
        std::map<std::string, Property<float>>       getFloatProperties() const;
        std::map<std::string, Property<bool>>        getBoolProperties() const;
        std::map<std::string, Property<std::string>> getStringProperties() const;
        std::map<std::string, Property<glm::vec3>>   getVec3Properties() const;

        // Check if property exists
        bool has(const std::string& property_name) const;
        
        // Remove the property if it exists
        void remove(const std::string& property_name);

        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;

        std::map<std::string, Property<int>> m_IntProperties;
        std::map<std::string, Property<float>> m_FloatProperties;
        std::map<std::string, Property<bool>> m_BoolProperties;
        std::map<std::string, Property<std::string>> m_StringProperties;
        std::map<std::string, Property<glm::vec3>> m_Vec3Properties;

        std::map<std::string, glm::vec2> m_Limits;
        std::map<std::string, float> m_Speeds;

        // Add property inside the derived class constructor
        // Please do not create properties with same names in different maps

        void addProperty(const std::string& property_name, int value, bool editable = true);
        void addProperty(const std::string& property_name, float value, bool editable = true);
        void addProperty(const std::string& property_name, bool value, bool editable = true);
        void addProperty(const std::string& property_name, const std::string& value, bool editable = true);
        void addProperty(const std::string& property_name, const glm::vec3& value, bool editable = true);
    
        // Set upper and lower limits for int, float or vec3 property (e.g. for editing via GUI)
        void setPropertyLimits(const std::string& property_name, const glm::vec2& limits);
        void setPropertyLimits(const std::string& property_name, float lower_limit, float upper_limit);
        
        // Set the property value editing speed which will be used in GUI editing
        void setPropertySpeed(const std::string& property_name, float speed);

        // Change object's editing capabilities
        void setPropertyEditable(const std::string& property_name, bool editable);
    };
}