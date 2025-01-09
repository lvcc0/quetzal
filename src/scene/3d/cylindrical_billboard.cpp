#include "scene/3d/cylindrical_billboard.h"

namespace qtzl
{
	CylindricalBillboard::CylindricalBillboard(const std::string& name, std::shared_ptr<Texture> texture)
		: Billboard(name, texture)
	{
	}

	void CylindricalBillboard::draw(const ShaderProgram& shader_program)
	{
        glm::vec3 projToTarget = glm::normalize(glm::vec3(this->m_Target.x - this->m_Position.x, 0.0f, this->m_Target.z - this->m_Position.z)); // projection of vector to target in the XZ plane
        m_Up = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), projToTarget); // flips the up vector if going the second half of the loop

        this->m_VerticalAngle = glm::acos(glm::dot(projToTarget, glm::vec3(0.0f, 0.0f, -1.0f)));
        if (this->m_VerticalAngle < 0.01 || this->m_VerticalAngle > 3.14)
            this->m_Up = glm::vec3(0.0f, 1.0f, 0.0f); // stability reasons

        this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, this->m_Position);
        this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, this->m_VerticalAngle, this->m_Up);
        this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, glm::vec3(this->m_Scale.x, this->m_Scale.y, 1.0f));

        shader_program.activateProgram();

        shader_program.setInt("material.texture_diffuse1", 0);
        glBindTexture(GL_TEXTURE_2D, this->m_Texture->getID());
        glActiveTexture(GL_TEXTURE0);

        // Convert local coordinates to world coordinates
        shader_program.setMat4("model", this->m_ModelMatrix);
        shader_program.setMat4("inversed", glm::inverse(this->m_ModelMatrix));

        this->m_ModelMatrix = glm::mat4(1.0f);

        this->m_VAO_uptr->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        this->m_VAO_uptr->unbind();
	}
}