#pragma once
#include "node_visitor.h"
#include "core/rendering/camera.h"
#include "scene/3d/cylindrical_billboard.h"
#include "scene/3d/spherical_billboard.h"

class CameraVisitor: public NodeVisitor {
public:
	void setCameraState(Camera& camera);
private:
	void visit(qtzl::SphericalBillboard& node) override;
	void visit(qtzl::CylindricalBillboard& node) override;
	glm::vec3 m_CameraPos;
};