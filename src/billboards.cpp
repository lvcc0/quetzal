#include "billboards.h"

// --- Cylindrical Billboard --- //
CylindricalBillboard::CylindricalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture>& texture)
    : m_pos(pos), m_size(size), m_texture(texture)
{
    m_vertices =
    {
        Vertex(glm::vec3( m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
    };

    setupBillboard();
}

CylindricalBillboard::~CylindricalBillboard()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void CylindricalBillboard::setupBillboard()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

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

void CylindricalBillboard::draw(std::shared_ptr<Shader>& shader, glm::vec3 object_pos)
{
    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(object_pos.x - m_pos.x, 0.0f, object_pos.z - m_pos.z)); // projection of vector to object in the XZ plane
    glm::vec3 up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop

    float angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    if (angle_in_rad < 0.01 || angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

    m_model_matrix = glm::translate(m_model_matrix, m_pos);
    m_model_matrix = glm::rotate(m_model_matrix, angle_in_rad, up);

    shader->setInt("material.texture_diffuse1", 0);

    glBindTexture(GL_TEXTURE_2D, m_texture->ID);
    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_model_matrix);
    shader->setMat4("inversed", glm::inverse(m_model_matrix));

    m_model_matrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void CylindricalBillboard::translate(glm::vec3 vector)
{
    this->m_pos = vector; // doing it like this because it already translates in the Draw function
}

void CylindricalBillboard::scale(glm::vec2 vector)
{
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(vector, 1.0f));
    this->m_size = vector;
}
// --- //

// --- Spherical Billboard --- //
SphericalBillboard::SphericalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture>& texture)
    : m_pos(pos), m_size(size), m_texture(texture)
{
    m_vertices =
    {
        Vertex(glm::vec3( m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
    };

    setupBillboard();
}

SphericalBillboard::~SphericalBillboard()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void SphericalBillboard::setupBillboard()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

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

void SphericalBillboard::draw(std::shared_ptr<Shader>& shader, glm::vec3 object_pos)
{
    glm::vec3 vector_to_obj = glm::normalize(object_pos - m_pos); // vector to the object
    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(object_pos.x - m_pos.x, 0.0f, object_pos.z - m_pos.z)); // projection of vector to object in the XZ plane
    
    glm::vec3 up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

    if (vector_to_obj.y < 0) right = -right; // flip the right vector if object is under the billboard

    float vert_angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    float hor_angle_in_rad = glm::acos(glm::dot(proj_to_obj, vector_to_obj));

    if (vert_angle_in_rad < 0.01 || vert_angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons
    if (!(hor_angle_in_rad > 0 || hor_angle_in_rad < 3.15)) hor_angle_in_rad = 0.0;             // stability reasons

    m_model_matrix = glm::translate(m_model_matrix, m_pos);
    
    m_model_matrix = glm::rotate(m_model_matrix, vert_angle_in_rad, up);
    m_model_matrix = glm::rotate(m_model_matrix, hor_angle_in_rad, right);

    shader->setInt("material.texture_diffuse1", 0);

    glBindTexture(GL_TEXTURE_2D, m_texture->ID);
    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_model_matrix);
    shader->setMat4("inversed", glm::inverse(m_model_matrix));

    m_model_matrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void SphericalBillboard::translate(glm::vec3 vector)
{
    this->m_pos = vector; // doing it like this because it already translates in the Draw function
}

void SphericalBillboard::scale(glm::vec2 vector)
{
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(vector, 1.0f));
    this->m_size = vector;
}
// --- //