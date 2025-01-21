#include "scene/3d/billboard.h"

namespace qtzl
{
	Billboard::Billboard(const std::string& name, std::shared_ptr<Texture> texture)
		: VisualNode3D(name), m_Texture(texture)
	{
		this->m_Type = Object::Type::BILLBOARD;

		this->addProperty("Target", glm::vec3(0.0f, 0.0f, 1.0f));

		this->setupRender();
	}

	void Billboard::set(const std::string& property_name, const glm::vec3& value)
	{
        if (!this->m_Vec3Properties.contains(property_name))
        {
            std::cerr << "ERROR::qtzl::Billboard::set: no such property \"" << property_name << "\"." << std::endl;
            return;
        }

        // Move children
		if (property_name == "Global position")
		{
			for (auto& entry : this->m_Children)
			{
				entry.second->set("Global position", value + entry.second->getVec3("Global position") - this->getVec3("Global position"));
			}
		}

        // NOTE: yeah im not sure if this will work as intended
        if (this->m_EditingLimits.contains(property_name))
        {
            this->m_Vec3Properties[property_name].value.x = std::min(std::max(value.x, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.y = std::min(std::max(value.y, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
            this->m_Vec3Properties[property_name].value.z = std::min(std::max(value.z, this->m_EditingLimits.at(property_name).x), this->m_EditingLimits.at(property_name).y);
        }
        else
        {
            this->m_Vec3Properties[property_name].value = value;
        }
	}

	void Billboard::setTarget(glm::vec3 target)
	{
		this->set("Target", target);
	}

	glm::vec3 Billboard::getTarget() const
	{
		return this->getVec3("Target");
	}

	glm::mat4 Billboard::getModelMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);

		matrix = glm::translate(matrix, this->m_Vec3Properties.at("Global position").value);
		matrix = glm::rotate(matrix, this->m_VerticalAngle, this->m_Up);
		matrix = glm::rotate(matrix, this->m_HorizontalAngle, this->m_Right);
		matrix = glm::scale(matrix, glm::vec3(this->m_Vec3Properties.at("Scale").value.x, this->m_Vec3Properties.at("Scale").value.y, 1.0f));

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