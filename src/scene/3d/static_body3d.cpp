#include "scene/3d/static_body3d.h"

namespace qtzl
{
    StaticBody3D::StaticBody3D(const std::string& name, std::shared_ptr<Mesh> mesh_sptr)
        : VisualNode3D(name), m_Mesh_sptr(mesh_sptr)
    {
        this->setupRender();
    }

    void StaticBody3D::render(std::shared_ptr<ShaderProgram> shader_program)
    {
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
    }

    void StaticBody3D::setupRender()
    {
        for (auto& submesh : this->m_Mesh_sptr->getSubMeshes())
        {
            submesh->setup();
        }
    }
}