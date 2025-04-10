#include "scene/3d/cylindrical_billboard.h"

namespace qtzl
{
    CylindricalBillboard::CylindricalBillboard(const std::string& name, std::shared_ptr<Texture> texture)
        : Billboard(name, texture)
    {
        this->m_Type = Object::Type::CYLINDRICAL_BILLBOARD;

        this->setPropertyEditable("Global rotation", false);
        this->setPropertyEditable("Rotation", false);
    }

    void CylindricalBillboard::render(const std::shared_ptr<ShaderProgram>& shader_program)
    {
        // projection of vector to target in the XZ plane
        glm::vec3 projToTarget = glm::normalize(glm::vec3(
            this->m_Vec3Properties.at("Target").value.x - this->m_Vec3Properties.at("Global position").value.x,
            0.0f,
            this->m_Vec3Properties.at("Target").value.z - this->m_Vec3Properties.at("Global position").value.z)
        );

        m_Up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), projToTarget); // flips the up vector if going the second half of the loop

        this->m_VerticalAngle = glm::acos(glm::dot(projToTarget, glm::vec3(0.0f, 0.0f, -1.0f)));
        
        if (this->m_VerticalAngle < 0.01 || m_VerticalAngle > 3.14)
            this->m_Up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->m_Vec3Properties.at("Global position").value);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, m_VerticalAngle, this->m_Up);
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, glm::vec3(this->m_Vec3Properties.at("Scale").value.x, this->m_Vec3Properties.at("Scale").value.y, 1.0f));

        shader_program->activateProgram();

        shader_program->setInt("material.texture_diffuse1", 0);
        glBindTexture(GL_TEXTURE_2D, this->m_Texture->getID());
        glActiveTexture(GL_TEXTURE0);

        // Convert local coordinates to world coordinates
        shader_program->setMat4("model", this->m_ModelMatrix);
        shader_program->setMat4("inversed", glm::inverse(this->m_ModelMatrix));

        this->m_ModelMatrix = glm::mat4(1.0f);

        // NOTE: only rotate the billboard, don't 1.0f it after, just rotate it backwards

        this->m_VAO_uptr->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        this->m_VAO_uptr->unbind();
    }
}