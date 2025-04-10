#include "physics_visitor.h"

void PhysicsVisitor::visit(qtzl::BoxCollision& node)
{
	visit_func(node);
}

void PhysicsVisitor::visit(qtzl::SphereCollision& node)
{
	visit_func(node);
}