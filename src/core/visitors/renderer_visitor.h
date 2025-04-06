#pragma once

#include "node_visitor.h"
#include "static/renderer.h"

class RendererVisitor : public NodeVisitor
{
	void visit(qtzl::StaticBody3D& node) override;
	void visit(qtzl::RigidBody3D& node) override;
	void visit(qtzl::Skybox& node) override;
	void visit(qtzl::PointLight3D& node) override;
	void visit(qtzl::DirectionalLight3D& node) override;
	void visit(qtzl::SpotLight3D& node) override;
	void visit(qtzl::CylindricalBillboard& node) override;
	void visit(qtzl::SphericalBillboard& node) override;
	void visit(qtzl::Model3D& node) override;
};