#include "scene/3d/billboard.h"

namespace qtzl
{
	Billboard::Billboard(const std::string& name, std::shared_ptr<Texture> texture)
		: VisualNode3D(name), m_Texture(texture)
	{
		this->setupRender();
	}

	void Billboard::setTarget(glm::vec3 target)
	{
		this->m_Target = target;
	}

	glm::vec3 Billboard::getTarget() const
	{
		return this->m_Target;
	}

	glm::mat4 Billboard::getModelMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, this->m_Position);
		matrix = glm::rotate(matrix, this->m_VerticalAngle, this->m_Up);
		matrix = glm::rotate(matrix, this->m_HorizontalAngle, this->m_Right);
		matrix = glm::scale(matrix, glm::vec3(this->m_Scale.x, this->m_Scale.y, 1.0f));

		return matrix;
	}

	void Billboard::render(std::shared_ptr<ShaderProgram> shader_program)
	{
		// TODO
	}

	void Billboard::setupRender()
	{
		this->m_VAO_uptr = std::make_unique<VAO>();
		this->m_VBO_uptr = std::make_unique<VBO>(&this->m_Vertices[0], this->m_Vertices.size() * sizeof(Vertex));

		VertexBufferLayout layout;
		layout.push(3, offsetof(Vertex, Position));
		layout.push(2, offsetof(Vertex, TexCoord));
		layout.push(3, offsetof(Vertex, Normal));

		this->m_VAO_uptr->addBuffer(*this->m_VBO_uptr, layout);
	}
}