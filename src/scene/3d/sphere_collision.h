#pragma once

#include "scene/3d/physics_node3d.h"

namespace qtzl {
	class SphereCollision : public PhysicsNode3D {
	public:
		SphereCollision(const std::string& name, const glm::vec3& pos, const float radius);
		virtual ~SphereCollision() = default;
	};
}