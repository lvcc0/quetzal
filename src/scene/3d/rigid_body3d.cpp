#include "scene/3d/rigid_body3d.h"

namespace qtzl
{
    RigidBody3D::RigidBody3D(const std::string& name, std::shared_ptr<Mesh> mesh_sptr)
        : VisualNode3D(name), m_Mesh_sptr(mesh_sptr)
    {
        this->m_Type = Object::Type::RIGID_BODY3D;

        this->setupRender();
    }

    void RigidBody3D::render(const std::shared_ptr<ShaderProgram>& shader_program)
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

    void RigidBody3D::translate(const glm::vec3& vector)
    {
        this->set("Global position", m_Vec3Properties.at("Global position").value + vector);
    }

    void RigidBody3D::rotate(const glm::vec3& vector)
    {
        this->set("Global rotation", m_Vec3Properties.at("Global rotation").value + vector);
    }

    void RigidBody3D::rotate(float radians, const glm::vec3& vector)
    {
        this->set("Global rotation", m_Vec3Properties.at("Global rotation").value + vector * radians);
    }

    void RigidBody3D::rotateDegrees(const glm::vec3& vector)
    {
        this->set("Global rotation", m_Vec3Properties.at("Global rotation").value + glm::radians(vector));
    }

    void RigidBody3D::rotateDegrees(float degrees, const glm::vec3& vector)
    {
        this->set("Global rotation", m_Vec3Properties.at("Global rotation").value + vector * glm::radians(degrees));
    }

    void RigidBody3D::scale(const glm::vec3& vector)
    {
        this->set("Scale", this->m_Vec3Properties.at("Scale").value * vector);
    }

    void RigidBody3D::setupRender()
    {
        for (auto& submesh : this->m_Mesh_sptr->getSubMeshes())
        {
            submesh->setup();
        }
    }
}