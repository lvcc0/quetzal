#include "scene/resources/mesh.h"

namespace qtzl
{
    Mesh::Mesh(const std::string& name, const std::string& path)
        : Resource(name, path)
    {
        // TODO:
        // 1. load the mesh with assimp
        // 2. setup mesh's vao, vbo, ebo
    }
}