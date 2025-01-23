#include "scene/3d/physics_node3d.h"

namespace qtzl {
	PhysicsNode3D::PhysicsNode3D(const std::string& name, const glm::vec3& pos) :
		Node3D(name)
	{
		this->m_Type = Object::Type::PHYSICS_NODE3D;
		Node3D::setGlobalPosition(pos);
		m_VisiblePart = nullptr;
	}

	const std::shared_ptr<RigidBody3D> const PhysicsNode3D::getVisiblePart()
	{
		return m_VisiblePart;
	}

	void PhysicsNode3D::setGlobalPosition(const glm::vec3& position)
	{
		this->set("Global position", position);

		for (auto& entry : this->m_Children)
		{
			entry.second->set("Global position", position + entry.second->getVec3("Global position"));
		}

		if (m_VisiblePart != nullptr)
			m_VisiblePart->set("Global position", position + m_VisiblePart->getVec3("Global position"));
	}
	void PhysicsNode3D::setGlobalRotation(const glm::vec3& radians)
	{
		this->set("Global rotation", radians);

		for (auto& entry : this->m_Children)
		{
			entry.second->set("Global rotation", radians + entry.second->getVec3("Global rotation"));
		}
		if(m_VisiblePart != nullptr)
			m_VisiblePart->set("Global rotation", radians + m_VisiblePart->getVec3("Global rotation"));
	}
	void PhysicsNode3D::setGlobalRotationDegrees(const glm::vec3& degrees)
	{
		this->set("Global rotation", glm::radians(degrees));

		for (auto& entry : this->m_Children)
		{
			entry.second->set("Global rotation", glm::radians(degrees) + entry.second->getVec3("Global rotation"));
		}
		if (m_VisiblePart != nullptr)
			m_VisiblePart->set("Global rotation", glm::radians(degrees) + m_VisiblePart->getVec3("Global rotation"));
	}
	void PhysicsNode3D::setScale(const glm::vec3& scale)
	{
		this->set("Scale", scale);
		if (m_VisiblePart != nullptr)
			m_VisiblePart->setScale(scale);
	}
	void PhysicsNode3D::set(const std::string& property_name, const glm::vec3& value)
	{
		Object::set(property_name, value);
		if (m_VisiblePart != nullptr)
		{
			m_VisiblePart->set(property_name, value);
		}
	}
	void PhysicsNode3D::set(const std::string& property_name, bool value)
	{
		Object::set(property_name, value);
		if (m_VisiblePart != nullptr)
		{
			m_VisiblePart->Object::set(property_name, value);
		}
	}
}