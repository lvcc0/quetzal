#include "billboard.h"

CylindricalBillboard::CylindricalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture> texture)
{
    this->m_pos = pos;
    this->m_size = size;
    this->m_texture = texture;

    m_vertices =
    {
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3( m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // lower left
    };
    
    m_indices =
    {
        0, 1, 3,
        3, 1, 2
    };
    
    setupBillboard();
}

void CylindricalBillboard::setupBillboard()
{
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Element Array Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    // Vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}

void CylindricalBillboard::Draw(std::shared_ptr<Shader>& shader, glm::vec3 object_pos)
{
    glm::vec3 proj_to_obj = glm::vec3(object_pos.x - m_pos.x, 0.0f, object_pos.z - m_pos.z); // projection of distance in the XZ plane
    glm::mat4 look_at = glm::lookAt(m_pos, m_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::vec3 up = glm::cross(glm::vec3(look_at[2][0], look_at[2][1], look_at[2][2]), proj_to_obj); // cross product of direction vector of look_at matrix and projection in the XZ plane

    float angle_in_rad = glm::acos(glm::dot(glm::normalize(proj_to_obj), glm::normalize(glm::vec3(look_at[2][0], look_at[2][1], look_at[2][2]))));
    if (angle_in_rad < 0.01 || angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

    m_model_matrix = glm::rotate(m_model_matrix, angle_in_rad, up);

    shader->setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture->ID);

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_model_matrix);
    shader->setMat4("inversed", glm::inverse(m_model_matrix));

    m_model_matrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void CylindricalBillboard::translate(glm::vec3 vector)
{
    m_model_matrix = glm::translate(m_model_matrix, vector);
    this->m_pos = vector; // for angle calculation in Draw() to work properly
}

void CylindricalBillboard::scale(glm::vec2 vector)
{
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(vector, 1.0f));
    this->m_size = vector;
}