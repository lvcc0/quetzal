#pragma once

// std
#include <vector>

#include "core/rendering/vertex.h"
#include "core/rendering/shader_program.h"

#include "scene/3d/visual_node3d.h"

#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

namespace qtzl
{
	// TODO: make it not abstract, so not rotating "3D sprites" can be made?
	// Abstract base class for Cylindrical and Spherical billboards
	class Billboard : public VisualNode3D
	{
	public:
		Billboard(const std::string& name, std::shared_ptr<Texture> texture);
		virtual ~Billboard() = default;
		
		void setTarget(glm::vec3 target);

		glm::vec3 getTarget() const;
		glm::mat4 getModelMatrix() const;

		void draw(const ShaderProgram& shader_program) override;

	protected:
		glm::mat4 m_ModelMatrix = glm::mat4(0.0f);

		std::shared_ptr<Texture> m_Texture;
		glm::vec3 m_Target = glm::vec3(0.0f);

		glm::vec3 m_Up = glm::vec3(0.0f);
		glm::vec3 m_Right = glm::vec3(0.0f);

		float m_HorizontalAngle = 0.0f; // in radians!
		float m_VerticalAngle = 0.0f; // in radians!

		std::vector<Vertex> m_Vertices =
		{
			Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // upper right
			Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // lower right
			Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
			Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // lower right
			Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
			Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
		};

		void setupRender() override;
	};
}