#pragma once

#include <functional>

// thirdparty
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace qtzl 
{
	class Node;
	class Node3D;
	class VisualNode3D;
	class StaticBody3D;
	class RigidBody3D;
	class PhysicsNode3D;
	class BoxCollision;
	class SphereCollision;
	class Skybox;
	class Light3D;
	class PointLight3D;
	class DirectionalLight3D;
	class SpotLight3D;
	class Billboard;
	class SphericalBillboard;
	class CylindricalBillboard;

	enum INTERACTION;
}
// Maybe make system which would be alerting if visit func not overloaded in some visitor
class NodeVisitor
{
public:
	virtual ~NodeVisitor() = default;

	virtual void visit(qtzl::Node& node) {};
	virtual void visit(qtzl::Node3D& node) {};
	virtual void visit(qtzl::VisualNode3D& node) {};
	virtual void visit(qtzl::StaticBody3D& node) {};
	virtual void visit(qtzl::RigidBody3D & node) {};
	virtual void visit(qtzl::PhysicsNode3D& node) {};
	virtual void visit(qtzl::BoxCollision& node) {};
	virtual void visit(qtzl::SphereCollision& node) {};
	virtual void visit(qtzl::Skybox& node) {};
	virtual void visit(qtzl::Light3D& node) {};
	virtual void visit(qtzl::PointLight3D& node) {};
	virtual void visit(qtzl::DirectionalLight3D& node) {};
	virtual void visit(qtzl::SpotLight3D& node) {};
	virtual void visit(qtzl::Billboard& node) {};
	virtual void visit(qtzl::SphericalBillboard& node) {};
	virtual void visit(qtzl::CylindricalBillboard& node) {};
};