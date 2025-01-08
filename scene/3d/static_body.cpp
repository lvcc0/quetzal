#include "scene/3d/static_body.h"

namespace qtzl
{
    StaticBody::StaticBody(const std::string& name, Mesh& mesh)
        : Node3D(name), m_Mesh(mesh)
    {
    }

    void StaticBody::draw(const ShaderProgram& shader_program)
    {
        // TODO
    }
}