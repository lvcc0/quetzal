#pragma once

#include "core/rendering/vertex_array.h"
#include "core/rendering/shader_program.h"

#include "scene/3d/node3d.h"

namespace qtzl
{
    // Base class for every 3D node that can be drawn on the scene
    class VisualNode3D : public Node3D
    {
    public:
        VisualNode3D(const std::string& name);
        virtual ~VisualNode3D() = default;

    protected:
        glm::mat4 m_ModelMatrix = glm::mat4(0.0f);

        virtual void setupRender() = 0;
    };
}