#include "sphere_collision.h"

qtzl::SphereCollision::SphereCollision(const std::string& name, const glm::vec3& pos, const float radius)
	: PhysicsNode3D(name, pos)
{
	this->m_Type = Object::Type::SPHERE_COLLISION;
	addProperty("Radius", radius);
	m_VisiblePart = std::make_shared<RigidBody3D>(name + "_visible_part", ResourceManager::getMesh("objects/catsphere/catsphere.obj"));
	m_VisiblePart->setGlobalPosition(pos);
}

