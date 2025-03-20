#include "scene/3d/spherical_billboard.h"

namespace qtzl
{
    SphericalBillboard::SphericalBillboard(const std::string& name, std::shared_ptr<Texture> texture)
        : Billboard(name, texture)
    {
        this->m_Type = Object::Type::SPHERICAL_BILLBOARD;

        //this->setPropertyEditable("Global rotation", false);
        //this->setPropertyEditable("Rotation", false);
    }

    void SphericalBillboard::render(const std::shared_ptr<ShaderProgram>& shader_program)
    {
        glm::vec3 vectorToTarget = glm::normalize(m_Target - m_GlobalPosition); // vector to the target

        // Projection of vector to target in different planes

        glm::vec3 targetYZ = glm::normalize(glm::vec3(
            0.0f,
            m_Target.y - m_GlobalPosition.y,
            m_Target.z - m_GlobalPosition.z)
        ); // for X axis

        glm::vec3 targetXZ = glm::normalize(glm::vec3(
            m_Target.x - m_GlobalPosition.x,
            0.0f,
            m_Target.z - m_GlobalPosition.z)
        ); // for Y axis
        
        this->m_Up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), targetXZ); // flips the up vector if going the second half of the loop, so it's either {0.0f, 1.0f, 0.0f} or {0.0f, -1.0f, 0.0f}
        this->m_Right = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), targetYZ); // flips the right vector if target is under the billboard, so it's either {1.0f, 0.0f, 0.0f} or {-1.0f, 0.0f, 0.0f}

        this->m_VerticalAngle = glm::acos(glm::dot(targetXZ, glm::vec3(0.0f, 0.0f, -1.0f)));
        this->m_HorizontalAngle = glm::acos(glm::dot(targetXZ, vectorToTarget));

        if (this->m_VerticalAngle < 0.01 || this->m_VerticalAngle > 3.14)
            this->m_Up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons
        if (!(this->m_HorizontalAngle > 0 || this->m_HorizontalAngle < 3.15))
            this->m_HorizontalAngle = 0.0; // stability reasons

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, m_GlobalPosition);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_VerticalAngle, this->m_Up);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_HorizontalAngle, this->m_Right);
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));

        shader_program->activateProgram();

        shader_program->setInt("material.texture_diffuse1", 0);

        glBindTexture(GL_TEXTURE_2D, this->m_Texture->getID());
        glActiveTexture(GL_TEXTURE0);

        // Convert local coordinates to world coordinates
        shader_program->setMat4("model", this->m_ModelMatrix);
        shader_program->setMat4("inversed", glm::inverse(this->m_ModelMatrix));

        this->m_ModelMatrix = glm::mat4(1.0f);

        this->m_VAO_uptr->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        this->m_VAO_uptr->unbind();
    }
    void SphericalBillboard::accept(NodeVisitor& visitor)
    {
        visitor.visit(*this);
    }
}