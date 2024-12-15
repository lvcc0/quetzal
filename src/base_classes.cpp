#include "base_classes.h"

Scene_Node::Scene_Node(std::string name)
    : m_Name(name)
{
}

Scene_Node::Scene_Node(const Scene_Node& obj)
    : m_Name(obj.getName() + "_copy")
{
}

Scene_Node::~Scene_Node()
{
}

Scene_Object::Scene_Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : m_Position(position), m_RotationDegrees(rotation), m_Scale(scale)
{
}

Scene_Object::Scene_Object(const Scene_Object& obj)
    : m_Position(obj.m_Position), m_RotationDegrees(obj.m_RotationDegrees), m_Scale(obj.m_Scale)
{
}

Scene_Object::~Scene_Object()
{
}

Renderable::Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
    : m_Vertices(vertices), m_Indices(indices)
{
}

Renderable::Renderable(const Renderable& obj)
    : m_Vertices(obj.m_Vertices), m_Indices(obj.m_Indices)
{
}

Renderable::~Renderable()
{
    vao_ptr = nullptr;
    vbo_ptr = nullptr;
    ibo_ptr = nullptr;
}