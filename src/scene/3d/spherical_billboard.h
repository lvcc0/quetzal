#pragma once

#include "scene/3d/billboard.h"

// TODO: fix the math with rotation and scaling

namespace qtzl
{
	// A flat plane rotating around YZ-axes relatively to the camera
	class SphericalBillboard : public Billboard
	{
	public:
		SphericalBillboard(const std::string& name, std::shared_ptr<Texture> texture);
		virtual ~SphericalBillboard() = default;

		void render(std::shared_ptr<ShaderProgram> shader_program) override;
	};
}