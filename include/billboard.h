#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "camera.h"

class Billboard
{
public:
    std::vector<Vertex> m_local_vertices;
    std::vector<Vertex> m_vertices;

    std::vector<unsigned int> m_indices;

    glm::mat4 m_model_matrix = glm::mat4(1.0f);

    Texture m_texture;

    glm::vec3 m_pos;
    glm::vec2 m_size;
    glm::mat4 m_transform;

    // Constructor
    Billboard(glm::vec3 pos, glm::vec2 size, std::string const& texture_path);

    // Draw billboard + update transform matrix
    void Draw(Shader& shader, glm::mat4 viewMatrix);

    // moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec2 vector);

private:
    unsigned int VAO, VBO, EBO;

    // Setup VAO, VBO, EBO
    void updateBuffers();
};

#endif