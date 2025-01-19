#pragma once

// std
#include <iostream>
#include <map>
#include <string>

// thirdparty
#include <glm/glm.hpp>

// TODO: add node-specific property editing limits
// TODO: removeProperty
// TODO: hasProperty

namespace qtzl
{
    // Base class for almost everything in the engine
    class Object
    {
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

        // TODO: check if the property exists in the getters

        // Set property if it exists

        virtual void set(const std::string& property_name, int property);
        virtual void set(const std::string& property_name, float property);
        virtual void set(const std::string& property_name, bool property);
        virtual void set(const std::string& property_name, const std::string& property);
        virtual void set(const std::string& property_name, const glm::vec3& property);

        // Get property

        int         getInt(const std::string& property_name) const;
        float       getFloat(const std::string& property_name) const;
        bool        getBool(const std::string& property_name) const;
        std::string getString(const std::string& property_name) const;
        glm::vec3   getVec3(const std::string& property_name) const;

        // Get all properties of type

        std::map<std::string, int>         getIntProperties() const;
        std::map<std::string, float>       getFloatProperties() const;
        std::map<std::string, bool>        getBoolProperties() const;
        std::map<std::string, std::string> getStringProperties() const;
        std::map<std::string, glm::vec3>   getVec3Properties() const;
        
        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;

        std::map<std::string, int> m_IntProperties;
        std::map<std::string, float> m_FloatProperties;
        std::map<std::string, bool> m_BoolProperties;
        std::map<std::string, std::string> m_StringProperties;
        std::map<std::string, glm::vec3> m_Vec3Properties;

        // Add property inside the derived class constructor

        void addProperty(const std::string& property_name, const int& property);
        void addProperty(const std::string& property_name, const float& property);
        void addProperty(const std::string& property_name, const bool& property);
        void addProperty(const std::string& property_name, const std::string& property);
        void addProperty(const std::string& property_name, const glm::vec3& property);
    };
}