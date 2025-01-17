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

        // NOTE: gotta rewrite this whole stuff sometime

        // Set property if it exists
        template <typename T> void set(const std::string& property_name, const T& property);
        template <> void set<int>(const std::string& property_name, const int& property);
        template <> void set<float>(const std::string& property_name, const float& property);
        template <> void set<bool>(const std::string& property_name, const bool& property);
        template <> void set<std::string>(const std::string& property_name, const std::string& property);
        template <> void set<glm::vec3>(const std::string& property_name, const glm::vec3& property);

        // Get property
        template <typename T> T get(const std::string& property_name) const;
        template <> int         get<int>(const std::string& property_name) const;
        template <> float       get<float>(const std::string& property_name) const;
        template <> bool        get<bool>(const std::string& property_name) const;
        template <> std::string get<std::string>(const std::string& property_name) const;
        template <> glm::vec3   get<glm::vec3>(const std::string& property_name) const;

        // Get all properties of type
        template <typename T> std::map<std::string, T> getProperties() const;
        template <> std::map<std::string, int>         getProperties<int>() const;
        template <> std::map<std::string, float>       getProperties<float>() const;
        template <> std::map<std::string, bool>        getProperties<bool>() const;
        template <> std::map<std::string, std::string> getProperties<std::string>() const;
        template <> std::map<std::string, glm::vec3>   getProperties<glm::vec3>() const;
        
        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;

        std::map<std::string, int> m_IntProperties;
        std::map<std::string, float> m_FloatProperties;
        std::map<std::string, bool> m_BoolProperties;
        std::map<std::string, std::string> m_StringProperties;
        std::map<std::string, glm::vec3> m_Vec3Properties;

        // Add property inside the constructor
        template <typename T> void addProperty(const std::string& property_name, const T& property);
        template <> void addProperty<int>(const std::string& property_name, const int& property);
        template <> void addProperty<float>(const std::string& property_name, const float& property);
        template <> void addProperty<bool>(const std::string& property_name, const bool& property);
        template <> void addProperty<std::string>(const std::string& property_name, const std::string& property);
        template <> void addProperty<glm::vec3>(const std::string& property_name, const glm::vec3& property);
    };
}