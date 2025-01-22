#pragma once

#include "scene/3d/physics_node3d.h"

namespace qtzl 
{
	class BoxCollision : public PhysicsNode3D
	{
	public:
		BoxCollision(const std::string& name, const glm::vec3& pos);
		virtual ~BoxCollision() = default;
	};
}