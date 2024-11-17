#include "billboards.h"

// --- Cylindrical Billboard --- //

CylindricalBillboard::CylindricalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload, std::string name)
    : Billboard(pos, scale, texture, verts, is_preload)
{
}

CylindricalBillboard::CylindricalBillboard(const CylindricalBillboard& obj):
    Billboard(obj)
{
}

void CylindricalBillboard::draw(const Shaders_pack& shaders)
{
    std::shared_ptr<Shader> main_shader = shaders.MAIN_SHADER;

    glm::vec3 proj_to_obj = glm::normalize(glm::vec3(m_Target.x - m_Position.x, 0.0f, m_Target.z - m_Position.z)); // projection of vector to object in the XZ plane
    up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), proj_to_obj); // flips the up vector if going the second half of the loop

    angle_in_rad = glm::acos(glm::dot(proj_to_obj, glm::vec3(0.0f, 0.0f, -1.0f)));
    if (angle_in_rad < 0.01 || angle_in_rad > 3.14) up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, angle_in_rad, up);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

    main_shader->activateShader();

    main_shader->setInt("material.texture_diffuse1", 0);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture->ID));
    GLCall(glActiveTexture(GL_TEXTURE0));
    
    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    vao_ptr->bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

    vao_ptr->unbind();
}
glm::mat4 CylindricalBillboard::getModelMatrix() const
{
    glm::mat4 return_matrix = glm::mat4(1.0f);
    return_matrix = glm::translate(return_matrix, m_Position);
    return_matrix = glm::rotate(return_matrix, angle_in_rad, up);
    return_matrix = glm::scale(return_matrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

    return return_matrix;
}
// --- //

// --- Spherical Billboard --- //

SphericalBillboard::SphericalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload, std::string name)
    : Billboard(pos, scale, texture, verts, is_preload, name)
{

}

SphericalBillboard::SphericalBillboard(const SphericalBillboard& obj):
    Billboard(obj)
{
}

void SphericalBillboard::draw(const Shaders_pack& shaders)
{
    std::shared_ptr<Shader> main_shader = shaders.MAIN_SHADER;

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
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

    main_shader->activateShader();

    main_shader->setInt("material.texture_diffuse1", 0);

    GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture->ID));
    GLCall(glActiveTexture(GL_TEXTURE0));

    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    m_ModelMatrix = glm::mat4(1.0f);

    vao_ptr->bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

    vao_ptr->unbind();
}
glm::mat4 SphericalBillboard::getModelMatrix() const
{
    glm::mat4 return_matrix = glm::mat4(1.0f);
    return_matrix = glm::translate(return_matrix, m_Position);
    return_matrix = glm::rotate(return_matrix, vert_angle_in_rad, up);
    return_matrix = glm::rotate(return_matrix, hor_angle_in_rad, right);
    return_matrix = glm::scale(return_matrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

    return return_matrix;
}
// --- //

// --- Billboard --- //
Billboard::Billboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload, std::string name)
    : m_Texture(texture), Renderable(verts), Scene_Node(name), Scene_Object(glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 0))
{
    if (!is_preload)
        setupRender();
}

Billboard::Billboard(const Billboard& obj):
    Renderable(obj), Scene_Node(obj), m_Texture(obj.m_Texture)
{
    setupRender();
}

inline void Billboard::setPosition(glm::vec3 pos)
{
    m_Position = pos;
}

inline void Billboard::setScale(glm::vec3 scale)
{
    m_Scale = glm::vec3(scale.x, scale.y, 0);
}

void Billboard::setupRender()
{
    // Vertex Array Object
    vao_ptr = std::make_unique<VAO>();

    // Vertex Buffer Object
    vbo_ptr = std::make_unique<VBO>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));

    VB_Vertex_Layout layout;
    layout.push<GLfloat>(3, offsetof(Vertex, Position));
    layout.push<GLfloat>(2, offsetof(Vertex, TexCoord));
    layout.push<GLfloat>(3, offsetof(Vertex, Normal));

    vao_ptr->addBuffer(*vbo_ptr, layout);
}
// --- //