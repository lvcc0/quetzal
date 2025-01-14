#include "scene/3d/static_body.h"

namespace qtzl
{
    StaticBody::StaticBody(const std::string& name, std::shared_ptr<Mesh> mesh_sptr)
        : VisualNode3D(name), m_Mesh_sptr(mesh_sptr)
    {
        this->setupRender();
    }

    void StaticBody::render(std::shared_ptr<ShaderProgram> shader_program)
    {
        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->m_GlobalPosition);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_GlobalRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_GlobalRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_GlobalRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, m_Scale);

        for (const auto& submesh : this->m_Mesh_sptr->getSubMeshes())
        {
            // bind appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;

            shader_program->activateProgram();

            for (unsigned int i = 0; i < submesh->getTextures().size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);

                std::string number;
                std::string type = submesh->getTextures()[i].getType();

                if (type == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (type == "texture_specular")
                    number = std::to_string(specularNr++);

                shader_program->setInt((type + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, submesh->getTextures()[i].getID());
            }

            // Convert local coordinates to world coordinates
            shader_program->setMat4("model", this->m_ModelMatrix);
            shader_program->setMat4("inversed", glm::inverse(this->m_ModelMatrix));

            // Draw mesh
            submesh->bindVAO();
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(submesh->getIndices().size()), GL_UNSIGNED_INT, 0);
            submesh->unbindVAO();

            glActiveTexture(GL_TEXTURE0);
        }

        this->m_ModelMatrix = glm::mat4(1.0f);
    }

    void StaticBody::setupRender()
    {
        for (auto& submesh : this->m_Mesh_sptr->getSubMeshes())
        {
            submesh->setup();
        }
    }
}