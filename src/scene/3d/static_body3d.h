#pragma once

#include "scene/3d/visual_node3d.h"

#include "scene/resources/mesh.h"

namespace qtzl
{
    // 3D body that can't be affected by external forces
    class StaticBody3D : public VisualNode3D
    {
    public:
        StaticBody3D(const std::string& name, std::shared_ptr<Mesh> mesh_sptr);
        virtual ~StaticBody3D() = default;

        void render(const std::shared_ptr<ShaderProgram>& shader_program) override;

    private:
        std::shared_ptr<Mesh> m_Mesh_sptr;

        void setupRender() override;
    };
}