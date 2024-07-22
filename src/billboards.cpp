#include "billboards.h"

// --- Cylindrical Billboard --- //

CylindricalBillboard::CylindricalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts)
    : Billboard(pos, scale, texture, verts)
{
    this->type = RenderableType::CYL_BILL;
}

void CylindricalBillboard::draw(std::vector<std::shared_ptr<Shader>>& shader_vector)
{
    std::shared_ptr<Shader> main_shader = shader_vector[to_underlying(ShaderType::MAIN)];

    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(m_Target.x - m_Position.x, 0.0f, m_Target.z - m_Position.z)); // projection of vector to object in the XZ plane
    up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop

    angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    if (angle_in_rad < 0.01 || angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, angle_in_rad, up);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 1.0f));

    main_shader->setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_Texture->ID);
    glActiveTexture(GL_TEXTURE0);
    
    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
glm::mat4 CylindricalBillboard::getModelMatrix()
{
    glm::mat4 return_matrix = glm::mat4(1.0f);
    return_matrix = glm::translate(return_matrix, m_Position);
    return_matrix = glm::rotate(return_matrix, angle_in_rad, up);
    return_matrix = glm::scale(return_matrix, glm::vec3(m_Scale, 1.0f));

    return return_matrix;
}
// --- //

// --- Spherical Billboard --- //

SphericalBillboard::SphericalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts)
    : Billboard(pos, scale, texture, verts)
{
    this->type = RenderableType::SPH_BILL;
}

void SphericalBillboard::draw(std::vector<std::shared_ptr<Shader>>& shader_vector)
{
    std::shared_ptr<Shader> main_shader = shader_vector[to_underlying(ShaderType::MAIN)];

    glm::vec3 vector_to_obj = glm::normalize(m_Target - m_Position); // vector to the object
    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(m_Target.x - m_Position.x, 0.0f, m_Target.z - m_Position.z)); // projection of vector to object in the XZ plane
    
    up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop
    right = glm::vec3(1.0f, 0.0f, 0.0f);

    if (vector_to_obj.y < 0) right = -right; // flip the right vector if object is under the billboard

    vert_angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    hor_angle_in_rad = glm::acos(glm::dot(proj_to_obj, vector_to_obj));

    if (vert_angle_in_rad < 0.01 || vert_angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons
    if (!(hor_angle_in_rad > 0 || hor_angle_in_rad < 3.15)) hor_angle_in_rad = 0.0;             // stability reasons

    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, vert_angle_in_rad, up);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, hor_angle_in_rad, right);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 1.0f));

    main_shader->setInt("material.texture_diffuse1", 0);

    glBindTexture(GL_TEXTURE_2D, m_Texture->ID);
    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
glm::mat4 SphericalBillboard::getModelMatrix()
{
    glm::mat4 return_matrix = glm::mat4(1.0f);
    return_matrix = glm::translate(return_matrix, m_Position);
    return_matrix = glm::rotate(return_matrix, vert_angle_in_rad, up);
    return_matrix = glm::rotate(return_matrix, hor_angle_in_rad, right);
    return_matrix = glm::scale(return_matrix, glm::vec3(m_Scale, 1.0f));

    return return_matrix;
}
// --- //

// --- Billboard --- //
Billboard::Billboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts)
    : m_Position(pos), m_Scale(scale), m_Texture(texture), Renderable(verts)
{
    this->type = RenderableType::BILLBOARD;
    setupRender();
}

void Billboard::translate(glm::vec3 vector)
{
    m_Position = vector;
}

void Billboard::scale(glm::vec2 vector)
{
    m_Scale = vector;
}

void Billboard::setupRender()
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
// --- //