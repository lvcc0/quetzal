#pragma once

#include "core/rendering/shader_program.h"

#include "scene/3d/visual_node3d.h"

#include "scene/resources/mesh.h"
#include "scene/resources/shader.h"

namespace qtzl
{
    // 3D body that can be affected by external forces
    class RigidBody : public VisualNode3D
    {
    public:
        RigidBody(const std::string& name, Mesh& mesh);
        virtual ~RigidBody() = default;

        void render(std::shared_ptr<ShaderProgram> shader_program) override;

    private:
        Mesh m_Mesh;
    };
}