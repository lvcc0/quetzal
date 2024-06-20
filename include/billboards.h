#pragma once

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <memory>

#include "model.h"
#include "vertex.h"
#include "camera.h"

// TODO: technically they're almost the same, so we can do something like inheritance here

class CylindricalBillboard
{
public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    glm::mat4 m_model_matrix = glm::mat4(1.0f);

    std::shared_ptr<Texture> m_texture;

    glm::vec3 m_pos;
    glm::vec2 m_size;
    
    // Constructor
    CylindricalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture>& texture);

    // Draw billboard and change it's model matrix
    void Draw(std::shared_ptr<Shader>& shader, glm::vec3 object_pos);

    // Moving in the world space
    void translate(glm::vec3 vector);
    void scale(glm::vec2 vector);

private:
    GLuint VAO, VBO, EBO;

    // Setup VAO, VBO, EBO
    void setupBillboard();
};

class SphericalBillboard
{
public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    glm::mat4 m_model_matrix = glm::mat4(1.0f);

    std::shared_ptr<Texture> m_texture;

    glm::vec3 m_pos;
    glm::vec2 m_size;

    // Constructor
    SphericalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture>& texture);

    // Draw billboard and change it's model matrix
    void Draw(std::shared_ptr<Shader>& shader, glm::vec3 player_pos);

    // Moving in the world space
    void translate(glm::vec3 vector);
    void scale(glm::vec2 vector);

private:
    GLuint VAO, VBO, EBO;

    // Setup VAO, VBO, EBO
    void setupBillboard();
};