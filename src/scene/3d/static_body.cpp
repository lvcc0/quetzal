#include "scene/3d/static_body.h"

namespace qtzl
{
    StaticBody::StaticBody(const std::string& name, std::shared_ptr<Mesh> mesh_sptr)
        : VisualNode3D(name), m_Mesh_sptr(mesh_sptr)
    {
        setupRender();
    }

    void StaticBody::render(std::shared_ptr<ShaderProgram> shader_program)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->m_GlobalPosition);
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, m_Scale);

        shader_program->activateProgram();

        for (unsigned int i = 0; i < this->m_Mesh_sptr->getTextures().size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string type = this->m_Mesh_sptr->getTextures()[i].getType();

            if (type == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (type == "texture_specular")
                number = std::to_string(specularNr++);

            shader_program->setInt((type + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, this->m_Mesh_sptr->getTextures()[i].getID());
        }

        // Convert local coordinates to world coordinates
        shader_program->setMat4("model", this->m_ModelMatrix);
        shader_program->setMat4("inversed", glm::inverse(this->m_ModelMatrix));

        this->m_ModelMatrix = glm::mat4(1.0f);

        // Draw mesh
        this->m_VAO_uptr->bind();
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->m_Mesh_sptr->getIndices().size()), GL_UNSIGNED_INT, 0);
        this->m_VAO_uptr->unbind();

        glActiveTexture(GL_TEXTURE0);
    }

    void StaticBody::setupRender()
    {
        this->m_VAO_uptr = std::make_unique<VAO>();
        this->m_VBO_uptr = std::make_unique<VBO>(&this->m_Mesh_sptr->getVertices()[0], this->m_Mesh_sptr->getVertices().size() * sizeof(Vertex));
        this->m_EBO_uptr = std::make_unique<EBO>(&this->m_Mesh_sptr->getIndices()[0], this->m_Mesh_sptr->getIndices().size() * sizeof(unsigned int));

        VertexBufferLayout layout;
        layout.push(3, offsetof(Vertex, Position));
        layout.push(2, offsetof(Vertex, TexCoord));
        layout.push(3, offsetof(Vertex, Normal));

        this->m_VAO_uptr->addBuffer(*this->m_VBO_uptr, layout);
    }
}