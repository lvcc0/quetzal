#pragma once

// std
#include <map>
#include <string>
#include <variant>

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

        struct Property
        {
            std::variant<int, float, bool, std::string, glm::vec3> value;
            enum Type { INT, FLOAT, BOOL, STRING, VEC3 } type;
        };

        Object(Type type);
        virtual ~Object() = default;

        void setProperty(const std::string& property_name, int value);
        void setProperty(const std::string& property_name, float value);
        void setProperty(const std::string& property_name, bool value);
        void setProperty(const std::string& property_name, const std::string& value);
        void setProperty(const std::string& property_name, const glm::vec3& value);

        Property getProperty(const std::string& property_name) const;
        std::map<std::string, Property> getProperties() const;

        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;
        std::map<std::string, Property> m_Properties;

        void addProperty(const std::string& property_name, Property& property);
        void addProperties(std::map<std::string, Property>& properties);
    };
}