#include "scene/3d/billboard.h"

namespace qtzl
{
	Billboard::Billboard(const std::string& name, std::shared_ptr<Texture>& texture)
		: Node3D(name), m_Texture(texture)
	{
	}

	void Billboard::setTarget(glm::vec3 target)
	{
		this->m_Target = target;
	}

	glm::vec3 Billboard::getTarget() const
	{
		return this->m_Target;
	}
}