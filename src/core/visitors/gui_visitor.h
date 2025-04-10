#pragma once

#include "node_visitor.h"
#include "static/gui.h"

// TODO: Make disabling buttons 
class GuiVisitor: public NodeVisitor{
	//void visit(qtzl::Node& node) override;
	void visit(qtzl::Node3D& node) override;
	void visit(qtzl::VisualNode3D& node) override;
	void visit(qtzl::StaticBody3D& node) override;
	void visit(qtzl::RigidBody3D& node) override;
	void visit(qtzl::PhysicsNode3D& node) override;
	void visit(qtzl::BoxCollision& node) override;
	void visit(qtzl::SphereCollision& node) override;
	void visit(qtzl::Skybox& node) override;
	void visit(qtzl::Light3D& node) override;
	void visit(qtzl::PointLight3D& node) override;
	void visit(qtzl::DirectionalLight3D& node) override;
	void visit(qtzl::SpotLight3D& node) override;
	void visit(qtzl::Billboard& node) override;
	void visit(qtzl::SphericalBillboard& node) override;
	void visit(qtzl::CylindricalBillboard& node) override;
	void visit(qtzl::Model3D& node) override;

	void showNode3D(qtzl::Node3D& node);
	void showNode3D(qtzl::VisualNode3D& node); // It is necessary because simple node doesnt update its model matrix(We dont update matrix every tick)
	void showNode3D(qtzl::RigidBody3D& node);
	void showNode3D(qtzl::StaticBody3D& node);
	void showLight3D(qtzl::Light3D& node);
};