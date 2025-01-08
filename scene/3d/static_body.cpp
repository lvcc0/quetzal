#include "scene/3d/static_body.h"

namespace qtzl
{
    StaticBody::StaticBody(const std::string& name, Mesh& mesh)
        : Node3D(name), m_Mesh(mesh)
    {
    }

    StaticBody::draw(const Shader& shader)
    {
        // TODO
    }
}