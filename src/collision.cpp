#include "collision.h"

Collision::Collision(const std::string name, const glm::vec3 begin_pos, const glm::vec3 rotation,
    std::vector<Vertex> verts, std::vector<unsigned int> indicies)
    : Scene_Object(begin_pos, rotation), Scene_Node(name), Renderable(verts, indicies)
{
    setupRender();
}

Collision::Collision(const Collision& obj)
    : Scene_Node(obj), Scene_Object(obj), Renderable(obj)
{ 
    setupRender();
}

void Collision::draw(const Shaders_pack& shaders)
{
    std::shared_ptr<Shader> main_shader = shaders.MAIN_SHADER;

    // Inheritor methods can be called
    this->setPosition();
    this->setScale();
    this->setRotationDegrees(glm::vec3(1.0, 0.0, 0.0));
    this->setRotationDegrees(glm::vec3(0.0, 1.0, 0.0));
    this->setRotationDegrees(glm::vec3(0.0, 0.0, 1.0));

    main_shader->activateShader(); // Using default shader

    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    vao_ptr->bind();
    GLCall(glDrawElements(GL_LINES, m_Indices.size(), GL_UNSIGNED_INT, 0));
    vao_ptr->unbind();

    m_ModelMatrix = glm::mat4(1.0f);
}

void Collision::setupRender()
{
    // Vertex Array Object
    vao_ptr = std::make_unique<VAO>();

    // Vertex Buffer Object
    vbo_ptr = std::make_unique<VBO>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));

    // Element Array Buffer
    ibo_ptr = std::make_unique<IBO>(&m_Indices[0], m_Indices.size());

    VB_Vertex_Layout layout;
    layout.push<GLfloat>(3, offsetof(Vertex, Position));

    vao_ptr->addBuffer(*vbo_ptr, layout);
}

// Another variants of initiating verts and indicies not working
RectCollision::RectCollision(const std::string name, const glm::vec3 begin_pos, const glm::vec3 rotation, glm::vec3 length)
    :Collision(name, begin_pos, rotation, 
        { Vertex(glm::vec3(-1.0, -1.0, -1.0)), //0
        Vertex(glm::vec3(1.0, -1.0, -1.0)), //1
        Vertex(glm::vec3(-1.0, 1.0, -1.0)), //2
        Vertex(glm::vec3(-1.0, -1.0, 1.0)), //3
        Vertex(glm::vec3(1.0, 1.0, -1.0)), //4
        Vertex(glm::vec3(-1.0, 1.0, 1.0)), //5
        Vertex(glm::vec3(1.0, -1.0, 1.0)), //6
        Vertex(glm::vec3(1.0, 1.0, 1.0)) }, //7
    { 0, 1, // 1 edge
    0, 2, // 2
    0, 3, // 3
    1, 4, // 4
    1, 6, // 5
    2, 4, // 6
    2, 5, // 7
    3, 5, // 8
    3, 6, // 9
    4, 7, // 10
    5, 7, // 11
    6, 7 // 12
        }), m_LengthPos(length)
{
}

inline void RectCollision::setPosition()
{
    m_AbsolutePosition = m_Position;
    // Parent pos
    auto parent_ptr = std::dynamic_pointer_cast<Scene_Object>(m_Parent_node.lock());
    if (parent_ptr != nullptr)
        m_AbsolutePosition += parent_ptr->getPosition();


    m_ModelMatrix = glm::translate(m_ModelMatrix, m_AbsolutePosition);
}

inline void RectCollision::setScale()
{
    m_AbsoluteScale = m_Scale;
    // Parent scale
    auto parent_ptr = std::dynamic_pointer_cast<Scene_Object>(m_Parent_node.lock());
    if (parent_ptr != nullptr)
        m_AbsoluteScale *= parent_ptr->getScale();

    m_ModelMatrix = glm::scale(m_ModelMatrix, m_AbsoluteScale);
}

inline void RectCollision::setRotationDegrees(const glm::vec3 rotation)
{
    m_AbsoluteRotationDegrees = m_RotationDegrees;
    // Parent rotation
    auto parent_ptr = std::dynamic_pointer_cast<Scene_Object>(m_Parent_node.lock());
    if (parent_ptr != nullptr)
        m_AbsoluteRotationDegrees += parent_ptr->getRotationDegrees();

    if (rotation == glm::vec3(1.0, 0.0, 0.0))
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_AbsoluteRotationDegrees.x), rotation);
    else if (rotation == glm::vec3(0.0, 1.0, 0.0))
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_AbsoluteRotationDegrees.y), rotation);
    else if (rotation == glm::vec3(0.0, 0.0, 1.0))
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_AbsoluteRotationDegrees.z), rotation);
    else
        __debugbreak();
}
