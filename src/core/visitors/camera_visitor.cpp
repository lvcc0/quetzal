#include "camera_visitor.h"

void CameraVisitor::setCameraState(Camera& camera)
{
	m_CameraPos = camera.m_Position;
}

void CameraVisitor::visit(qtzl::SphericalBillboard& node)
{
	node.setTarget(m_CameraPos);
}

void CameraVisitor::visit(qtzl::CylindricalBillboard& node)
{
	node.setTarget(m_CameraPos);
}
