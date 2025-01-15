#include "scene/3d/skybox.h"

namespace qtzl
{
    Skybox::Skybox(const std::string& name, unsigned int texture)
        : Node3D(name), m_Texture(texture)
    {
        this->m_Renderable = true;
        this->m_Type = Variant::Type::SKYBOX;

        setupRender();
    }

    void Skybox::render(std::shared_ptr<ShaderProgram> shader_program)
    {
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        
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
}