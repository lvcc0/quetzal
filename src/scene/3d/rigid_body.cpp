#include "scene/3d/rigid_body.h"

namespace qtzl
{
    RigidBody::RigidBody(const std::string& name, Mesh& mesh)
        : VisualNode3D(name), m_Mesh(mesh)
    {
    }

    void RigidBody::render(std::shared_ptr<ShaderProgram> shader_program)
    {
        // TODO
    }
}