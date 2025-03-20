#include "scene/3d/skybox.h"

namespace qtzl
{
    Skybox::Skybox(const std::string& name, unsigned int texture)
        : VisualNode3D(name), m_Texture(texture)
    {
        this->m_Type = Object::Type::SKYBOX;

        //this->removeProperty("Global position");
        //this->removeProperty("Global rotation");
        //this->removeProperty("Position");
        //this->removeProperty("Rotation");
        //this->removeProperty("Scale");

        this->setupRender();
    }

    void Skybox::render(const std::shared_ptr<ShaderProgram>& shader_program)
    {
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_Texture);

        this->m_VAO_uptr->bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        this->m_VAO_uptr->unbind();

        glDepthFunc(GL_LESS); // set depth function back to default
    }

    void Skybox::setupRender()
    {
        this->m_VAO_uptr = std::make_unique<VAO>();
        this->m_VBO_uptr = std::make_unique<VBO>(&this->m_Vertices[0], this->m_Vertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.push(3, 0);

        this->m_VAO_uptr->addBuffer(*this->m_VBO_uptr, layout, 3 * sizeof(float));
    }
    void Skybox::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}