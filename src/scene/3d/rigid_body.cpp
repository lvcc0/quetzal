#include "scene/3d/rigid_body.h"

namespace qtzl
{
    RigidBody::RigidBody(const std::string& name, Mesh& mesh)
        : Node3D(name), m_Mesh(mesh)
    {
    }

    void RigidBody::draw(const ShaderProgram& shader_program)
    {
        // TODO
    }
}