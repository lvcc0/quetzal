#pragma once

// TODO: make checkd for renderable and stuff in here

namespace qtzl
{
    class Variant
    {
    public:
        enum Type
        {
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

        enum ShaderProgramType
        {
            DEFAULT_SP,
            STENCIL_SP,
            SKYBOX_SP
        };
    };
}