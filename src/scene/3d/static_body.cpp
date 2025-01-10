#include "scene/3d/static_body.h"

namespace qtzl
{
    StaticBody::StaticBody(const std::string& name, Mesh& mesh)
        : VisualNode3D(name), m_Mesh(mesh)
    {
    }

    void StaticBody::render(std::shared_ptr<ShaderProgram> shader_program)
    {
        // TODO
    }
}