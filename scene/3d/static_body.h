#pragma once

#include "core/rendering/shader_program.h"

#include "scene/3d/node3d.h"

#include "scene/resources/mesh.h"
#include "scene/resources/shader.h"

namespace qtzl
{
    // 3D body that can't be affected by external forces
    class StaticBody : public Node3D
    {
    public:
        StaticBody(const std::string& name, Mesh& mesh);
        virtual ~StaticBody() = default;

        void draw(const ShaderProgram& shader_program);

    private:
        Mesh m_Mesh;
    };
}