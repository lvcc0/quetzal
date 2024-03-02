#include "billboard.h"

Billboard::Billboard(glm::vec3 pos, glm::vec2 size, std::string const& texture_path)
{
    m_pos = pos;
    m_size = size;
    m_transform = glm::mat4(1.0f);

    m_edges =
    {
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // upper left
        Vertex(glm::vec3(-m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // lower left
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // lower right
        Vertex(glm::vec3( m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // upper right
    };

    m_vertices = m_edges;

    m_indices =
    {
        0, 1, 2,
        2, 3, 0
    };

    Texture texture;
    texture.id = TextureFromFile(texture_path.c_str(), "");
    texture.type = "texture_diffuse";
    texture.path = texture_path.c_str();

    m_texture = texture;

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

void Billboard::Draw(Shader& shader, Camera cam)
{
    for (unsigned int i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i].Position = m_pos
            + glm::vec3(cam.getViewMatrix()[0][0], cam.getViewMatrix()[1][0], cam.getViewMatrix()[2][0]) * m_edges[i].Position.x
            + glm::vec3(cam.getViewMatrix()[0][1], cam.getViewMatrix()[1][1], cam.getViewMatrix()[2][1]) * m_edges[i].Position.y;
    }
    
    setupBillboard();

    shader.setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture.id);

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader.setMat4("model", m_model_matrix);
    shader.setMat4("inversed", glm::inverse(m_model_matrix));

    m_model_matrix = glm::mat4(1.0f);

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