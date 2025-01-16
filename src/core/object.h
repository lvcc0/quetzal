#pragma once

// std
#include <map>
#include <string>
#include <variant>

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

        void setProperty(const std::string& property_name);

        virtual std::map<std::string, std::variant<int, float, std::string>> getProperties() const;

        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;

        // NOTE: i don't really know if this whole std::optional thing fits here
        std::map<std::string, std::variant<int, float, std::string>> m_Properties;
    };
}