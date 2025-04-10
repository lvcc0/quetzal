#pragma once

#include "node_visitor.h"
#include "static/physics.h"
#include <type_traits>
#include "core/debugger.h"

#include <variant>

#define PHYSICS_TYPES_VARIANT std::variant<std::shared_ptr<qtzl::BoxCollision>, std::shared_ptr<qtzl::SphereCollision>>

// Use for std::visit
template <class... Fs> struct Overload : Fs... { using Fs::operator()...; };
template <class... Fs> Overload(Fs...) -> Overload<Fs...>;

class PhysicsVisitor : public NodeVisitor
{
	std::vector<PHYSICS_TYPES_VARIANT> vecOfNodes3DPhysical;

	void visit(qtzl::BoxCollision& node) override;
	void visit(qtzl::SphereCollision& node) override;

	template<class T>
	void visit_func(T& obj);

public:
	inline void clearPhysNodes() { vecOfNodes3DPhysical.clear(); };
};

template<class T>
inline void PhysicsVisitor::visit_func(T& node)
{
	T* node_ptr = &node;
	for (auto& phys_node : vecOfNodes3DPhysical)
	{
		std::visit(
			Overload{
				[node_ptr](std::shared_ptr<qtzl::BoxCollision> second) {
				auto& not_const_node = const_cast<T&>(*node_ptr);
				Physics::doCollisions(not_const_node, *second);
			},
				[node_ptr](std::shared_ptr<qtzl::SphereCollision> second) {
				auto& not_const_node = const_cast<T&>(*node_ptr);
				Physics::doCollisions(not_const_node, *second);
			}
			},
			phys_node);
	}
	vecOfNodes3DPhysical.push_back(std::make_shared<T>(node));
}
