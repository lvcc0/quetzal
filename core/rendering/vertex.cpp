#include "core/rendering/vertex.h"

Vertex::Vertex(glm::vec3 position, glm::vec2 tex_coord, glm::vec3 normal)
    : Position(position), TexCoord(tex_coord), Normal(normal)
{
}