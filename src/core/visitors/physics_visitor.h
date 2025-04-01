#pragma once

#include "node_visitor.h"
#include "static/physics.h"
#include <type_traits>

#include <variant>

#define PHYSICS_TYPES_VARIANT std::variant<qtzl::BoxCollision*, qtzl::SphereCollision*>

// Use for std::visit
template <class... Fs> struct Overload : Fs... { using Fs::operator()...; };
template <class... Fs> Overload(Fs...) -> Overload<Fs...>;

class PhysicsVisitor : public NodeVisitor
{
	std::vector<PHYSICS_TYPES_VARIANT> physicsNodes;

	void visit(qtzl::BoxCollision& node) override;
	void visit(qtzl::SphereCollision& node) override;

	static void placeholder(PHYSICS_RET_TYPE) { };
public:
	inline void clearPhysNodes() { physicsNodes.clear(); };
};