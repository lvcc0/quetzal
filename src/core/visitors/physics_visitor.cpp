#include "physics_visitor.h"

void PhysicsVisitor::visit(qtzl::BoxCollision& node)
{
	for (const auto& phys_node : vecOfNodes3DPhysical)
	{
		std::visit(
			Overload{
				[node](qtzl::BoxCollision* second) {placeholder(Physics::areColliding(node, *second)); },
				[node](qtzl::SphereCollision* second) {placeholder(Physics::areColliding(node, *second)); }
			},
			phys_node);
	}
	vecOfNodes3DPhysical.push_back(&node);
}

void PhysicsVisitor::visit(qtzl::SphereCollision& node)
{
	for (const auto& phys_node : vecOfNodes3DPhysical)
	{
		std::visit(
			Overload{
				[node](qtzl::BoxCollision* second) {placeholder(Physics::areColliding(node, *second)); },
				[node](qtzl::SphereCollision* second) {placeholder(Physics::areColliding(node, *second)); }
			},
			phys_node);
	}
	vecOfNodes3DPhysical.push_back(&node);
}