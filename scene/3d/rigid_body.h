#pragma once

#include "scene/3d/node3d.h"

#include "scene/resources/mesh.h"
#include "scene/resources/shader.h"

namespace qtzl
{
    // 3D body that can be affected by external forces
    class RigidBody : public Node3D
    {
    public:
        RigidBody(const std::string& name, Mesh& mesh);
        virtual ~RigidBody() = default;

        void draw(const Shader& shader);

    private:
        Mesh m_Mesh;
    };
}