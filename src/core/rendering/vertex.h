#pragma once

// thirdparty
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// A single vertex in 3D space
class Vertex
{
public:
    Vertex(glm::vec3 position = glm::vec3(0.0f), glm::vec2 tex_coord = glm::vec2(0.0f), glm::vec3 normal = glm::vec3(0.0f));
    
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
};