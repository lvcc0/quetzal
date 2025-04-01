#include "physics_visitor.h"

void PhysicsVisitor::visit(qtzl::BoxCollision& node)
{
	for (PHYSICS_TYPES_VARIANT item : physicsNodes)
	{
        std::visit(
            Overload{
				[node](qtzl::BoxCollision* second) {placeholder(Physics::areColliding(node, *second)); },
				[node](qtzl::SphereCollision* second) {placeholder(Physics::areColliding(node, *second)); }
            },
            item);
	}
	physicsNodes.push_back(&node);
}

void PhysicsVisitor::visit(qtzl::SphereCollision& node)
{
	for (PHYSICS_TYPES_VARIANT item : physicsNodes)
	{
		std::visit(
			Overload{
				[node](qtzl::BoxCollision* second) {placeholder(Physics::areColliding(node, *second)); },
				[node](qtzl::SphereCollision* second) {placeholder(Physics::areColliding(node, *second)); }
			},
			item);
	}
	physicsNodes.push_back(&node);
}
