#pragma once

#include "scene/3d/node3d.h"

#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

namespace qtzl
{
	// TODO: make it not abstract, so not rotating "3D sprites" can be made?
	// TODO: inherit from some visual class, not Node3D itself?
	// Abstract base class for Cylindrical and Spherical billboards
	class Billboard : public Node3D
	{
	public:
		Billboard(const std::string& name, std::shared_ptr<Texture>& texture);
		virtual ~Billboard() = default;
		
		void setTarget(glm::vec3 target);

		glm::vec3         getTarget() const;
		virtual glm::mat4 getModelMatrix() const;

		virtual void draw(const Shader& shader) = 0;

	protected:
		std::shared_ptr<Texture> m_Texture;
		glm::vec3 m_Target = glm::vec3(0.0f);

		glm::vec3 m_Up = glm::vec3(0.0f);
		glm::vec3 m_Right = glm::vec3(0.0f);

		float m_HorizontalAngle = 0.0f; // in radians!
		float m_VerticalAngle = 0.0f; // in radians!
	};
}