#include "scene/3d/spherical_billboard.h"

namespace qtzl
{
	SphericalBillboard::SphericalBillboard(const std::string& name, std::shared_ptr<Texture>& texture)
		: Billboard(name, texture)
	{
	}

	glm::mat4 SphericalBillboard::getModelMatrix() const
	{
		// TODO
	}

	void SphericalBillboard::draw(const Shader& shader)
	{
		// TODO
	}
}