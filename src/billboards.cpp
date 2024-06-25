#include "billboards.h"

// --- Cylindrical Billboard --- //
CylindricalBillboard::CylindricalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture)
    : m_Position(pos), m_Scale(scale), m_Texture(texture)
{
    m_Vertices =
    {
        Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
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
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

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

void CylindricalBillboard::draw(std::shared_ptr<Shader>& shader, glm::vec3 target_position)
{
    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(target_position.x - m_Position.x, 0.0f, target_position.z - m_Position.z)); // projection of vector to object in the XZ plane
    glm::vec3 up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop

    float angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    if (angle_in_rad < 0.01 || angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, angle_in_rad, up);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 1.0f));

    shader->setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_Texture->ID);
    glActiveTexture(GL_TEXTURE0);
    
    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_ModelMatrix);
    shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void CylindricalBillboard::translate(glm::vec3 vector)
{
    m_Position = vector;
}

void CylindricalBillboard::scale(glm::vec2 vector)
{
    m_Scale = vector;
}
// --- //

// --- Spherical Billboard --- //
SphericalBillboard::SphericalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture)
    : m_Position(pos), m_Scale(scale), m_Texture(texture)
{
    m_Vertices =
    {
        Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
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
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

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

void SphericalBillboard::draw(std::shared_ptr<Shader>& shader, glm::vec3 target_position)
{
    glm::vec3 vector_to_obj = glm::normalize(target_position - m_Position); // vector to the object
    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(target_position.x - m_Position.x, 0.0f, target_position.z - m_Position.z)); // projection of vector to object in the XZ plane
    
    glm::vec3 up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

    if (vector_to_obj.y < 0) right = -right; // flip the right vector if object is under the billboard

    float vert_angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    float hor_angle_in_rad = glm::acos(glm::dot(proj_to_obj, vector_to_obj));

    if (vert_angle_in_rad < 0.01 || vert_angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons
    if (!(hor_angle_in_rad > 0 || hor_angle_in_rad < 3.15)) hor_angle_in_rad = 0.0;             // stability reasons

    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, vert_angle_in_rad, up);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, hor_angle_in_rad, right);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 1.0f));

    shader->setInt("material.texture_diffuse1", 0);

    glBindTexture(GL_TEXTURE_2D, m_Texture->ID);
    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_ModelMatrix);
    shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void SphericalBillboard::translate(glm::vec3 vector)
{
    m_Position = vector;
}

void SphericalBillboard::scale(glm::vec2 vector)
{
    m_Scale = vector;
}
// --- //