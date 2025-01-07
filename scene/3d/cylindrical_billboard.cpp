#include "scene/3d/cylindrical_billboard.h"

namespace qtzl
{
	CylindricalBillboard::CylindricalBillboard(const std::string& name, std::shared_ptr<Texture>& texture)
		: Billboard(name, texture)
	{
	}

	glm::mat4 CylindricalBillboard::getModelMatrix() const
	{
		// TODO
	}

	void CylindricalBillboard::draw(const Shader& shader)
	{
        // TODO
	}
}