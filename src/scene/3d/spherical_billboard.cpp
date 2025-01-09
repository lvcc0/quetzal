#include "scene/3d/spherical_billboard.h"

namespace qtzl
{
	SphericalBillboard::SphericalBillboard(const std::string& name, std::shared_ptr<Texture>& texture)
		: Billboard(name, texture)
	{
	}

	glm::mat4 SphericalBillboard::getModelMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, this->m_Position);
		matrix = glm::rotate(matrix, this->m_VerticalAngle, this->m_Up);
		matrix = glm::rotate(matrix, this->m_HorizontalAngle, this->m_Right);
		matrix = glm::scale(matrix, glm::vec3(this->m_Scale.x, this->m_Scale.y, 1.0f));

		return matrix;
	}

	void SphericalBillboard::draw(const ShaderProgram& shader_program)
	{
		// TODO
	}
}