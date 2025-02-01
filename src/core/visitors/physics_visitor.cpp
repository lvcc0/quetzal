#include "physics_visitor.h"

void PhysicsVisitor::visit(qtzl::BoxCollision& node)
{
	if(!Physics::checkPhysicsNode(&node))
		Physics::addPhysicsNode(&node);
}

void PhysicsVisitor::visit(qtzl::SphereCollision& node)
{
	if (!Physics::checkPhysicsNode(&node))
		Physics::addPhysicsNode(&node);
}

void PhysicsVisitor::visit(qtzl::PhysicsNode3D& node)
{
	if (!Physics::checkPhysicsNode(&node))
		Physics::addPhysicsNode(&node);
}
