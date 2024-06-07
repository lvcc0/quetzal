#include "billboard.h"

Billboard::Billboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture> texture)
{
    this->m_pos = pos;
    this->m_size = size;
    this->m_texture = texture;

    // TODO: with culling back faces billboards aren`t working, except changing glFrontFace(GL_CCW) to glFrontFace(CW)
    // It means that billboard considered as back face
    m_vertices =
    {
        Vertex(glm::vec3(0.0f, -m_size[0] / 2.0f,  m_size[1] / 2.0f), glm::vec2(0.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // upper left
        Vertex(glm::vec3(0.0f, -m_size[0] / 2.0f, -m_size[1] / 2.0f), glm::vec2(0.0f,  0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // lower left
        Vertex(glm::vec3(0.0f,  m_size[0] / 2.0f, -m_size[1] / 2.0f), glm::vec2(1.0f,  0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // lower right
        Vertex(glm::vec3(0.0f,  m_size[0] / 2.0f,  m_size[1] / 2.0f), glm::vec2(1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // upper right
    };

    m_indices =
    {
        0, 1, 2,
        2, 3, 0
    };

    setupBillboard();
}

void Billboard::setupBillboard()
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

void Billboard::Draw(std::shared_ptr<Shader>& shader, glm::vec3 player_pos)
{
    glm::vec3 dir_from_player = m_pos - player_pos;
    float distance2D = glm::sqrt(dir_from_player.x * dir_from_player.x + dir_from_player.y * dir_from_player.y);

    float theta = glm::atan(dir_from_player.y, dir_from_player.x); // x, y
    float phi = glm::atan(-dir_from_player.z, distance2D);         // z, distance

    // this math doesn't work now (apparently it shows the back of the billboard)
    // can't use transforms outside of here

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_pos);
    m_model_matrix = m_model_matrix * glm::eulerAngleXYZ(0.0f, 0.0f, theta) * glm::eulerAngleXYZ(0.0f, phi, 0.0f);

    shader->setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture->ID);

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_model_matrix);
    shader->setMat4("inversed", glm::inverse(m_model_matrix));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Billboard::translate(glm::vec3 vector)
{
    m_model_matrix = glm::translate(m_model_matrix, vector);
}

void Billboard::scale(glm::vec2 vector)
{
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(vector, 1.0f));
}