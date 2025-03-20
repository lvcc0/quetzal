#include "scene/3d/billboard.h"

namespace qtzl
{
    Billboard::Billboard(const std::string& name, std::shared_ptr<Texture> texture)
        : VisualNode3D(name), m_Texture(texture)
    {
        this->m_Type = Object::Type::BILLBOARD;

        m_Target = glm::vec3(0.0f, 0.0f, 1.0f);

        this->setupRender();
    }

    //void Billboard::set(const std::string& property_name, const glm::vec3& value)
    //{
    //    // Move children
    //    if (property_name == "Global position")
    //    {
    //        for (auto& entry : this->m_Children)
    //        {
    //            entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
    //        }
    //    }

        // NOTE: yeah im not sure if this will work as intended
        //if (this->m_EditingLimits.contains(property_name))
        //{
        //    this->m_Vec3Properties[property_name].value.x = std::min(std::max(value.x, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        //    this->m_Vec3Properties[property_name].value.y = std::min(std::max(value.y, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        //    this->m_Vec3Properties[property_name].value.z = std::min(std::max(value.z, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        //}
        //else
        //{
        //    this->m_Vec3Properties[property_name].value = value;
        //}
    //}

    void Billboard::setTarget(glm::vec3 target)
    {
        m_Target = target;
    }

    glm::vec3 Billboard::getTarget() const
    {
        return m_Target;
    }

    glm::mat4 Billboard::getModelMatrix() const
    {
        glm::mat4 matrix = glm::mat4(1.0f);

        matrix = glm::translate(matrix, m_GlobalPosition);
        matrix = glm::rotate(matrix, this->m_VerticalAngle, this->m_Up);
        matrix = glm::rotate(matrix, this->m_HorizontalAngle, this->m_Right);
        matrix = glm::scale(matrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

        return matrix;
    }

    void Billboard::render(const std::shared_ptr<ShaderProgram>& shader_program)
    {
        // TODO
    }

    void Billboard::setupRender()
    {
        this->m_VAO_uptr = std::make_unique<VAO>();
        this->m_VBO_uptr = std::make_unique<VBO>(&this->m_Vertices[0], this->m_Vertices.size() * sizeof(Vertex));

        VertexBufferLayout layout;
        layout.push(3, offsetof(Vertex, Position));
        layout.push(2, offsetof(Vertex, TexCoord));
        layout.push(3, offsetof(Vertex, Normal));

        this->m_VAO_uptr->addBuffer(*this->m_VBO_uptr, layout);
    }
    void Billboard::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}