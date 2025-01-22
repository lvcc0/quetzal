#include "box_collision.h"

qtzl::BoxCollision::BoxCollision(const std::string& name, const glm::vec3& pos):
	PhysicsNode3D(name, pos)
{
	this->m_Type = Object::Type::BOX_COLLISION;
	addProperty("Size", glm::vec3(1.));
	m_VisiblePart = std::make_shared<RigidBody3D>(name + "_visible_part", ResourceManager::getMesh("objects/catcube/catcube.obj"));
}
