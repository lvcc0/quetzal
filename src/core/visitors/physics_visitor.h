#pragma once

#include "node_visitor.h"
#include "static/physics.h"

class PhysicsVisitor : public NodeVisitor
{
	void visit(qtzl::BoxCollision& node) override;
	void visit(qtzl::SphereCollision& node) override;
	void visit(qtzl::PhysicsNode3D& node) override;
};