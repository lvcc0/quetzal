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
                    MODEL3D,
                    RIGID_BODY3D,
                    STATIC_BODY3D,
            LIGHT3D,
                DIRECTIONAL_LIGHT3D,
                POINT_LIGHT3D,
                SPOT_LIGHT3D,
            PHYSICS_NODE3D,
                BOX_COLLISION,
                SPHERE_COLLISION,
            SKYBOX
        };

        Object(Type type);
        virtual ~Object() = default;

        // Get editing limits of the property
        glm::vec2 getEditingLimits(const std::string& property_name) const;

        // Get editing speed of the property
        float getEditingSpeed(const std::string& property_name) const;

        Type getType() const;
        bool isType(Type type) const;

    protected:
        Type m_Type;

        std::map<std::string, glm::vec2> m_EditingLimits;
        std::map<std::string, float> m_EditingSpeeds;
    };
}