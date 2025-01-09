#pragma once

#include "scene/3d/billboard.h"

namespace qtzl
{
	// A flat plane rotating around YZ-axes relatively to the camera
	class SphericalBillboard : public Billboard
	{
	public:
		SphericalBillboard(const std::string& name, std::shared_ptr<Texture> texture);
		virtual ~SphericalBillboard() = default;

		// Draw billboard and change it's model matrix
		void draw(const ShaderProgram& shader_program) override;
	};
}