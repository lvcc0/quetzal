#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

enum class VERTEX_VARIABLE{ NONE ,POS, TEX, NOR };

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;

    Vertex(glm::vec3 pos = glm::vec3(0., 0., 0.), glm::vec2 texCoord = glm::vec2(0., 0.), glm::vec3 norm = glm::vec3(0., 0., 0.))
        : Position(pos), TexCoord(texCoord), Normal(norm)
    { /* empty */ }

};