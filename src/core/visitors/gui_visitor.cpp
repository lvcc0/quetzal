#include "gui_visitor.h"

void GuiVisitor::visit(qtzl::Node3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());
    
    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::VisualNode3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::StaticBody3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::RigidBody3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::PhysicsNode3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::BoxCollision& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::SphereCollision& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showNode3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::Skybox& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());
    ImGui::Checkbox("Visible", &node.m_IsVisible);
    ImGui::End();

}

void GuiVisitor::visit(qtzl::Light3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());

    showLight3D(node);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::PointLight3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());
    showLight3D(node);
    ImGui::DragFloat3("Global position", glm::value_ptr(node.m_GlobalPosition));
    ImGui::DragFloat("Constant", &node.m_Constant);
    ImGui::DragFloat("Linear", &node.m_Linear);
    ImGui::DragFloat("Quadratic", &node.m_Quadratic);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::DirectionalLight3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());
    showLight3D(node);
    ImGui::DragFloat3("Direction", glm::value_ptr(node.m_Direction));
    ImGui::End();
}

void GuiVisitor::visit(qtzl::SpotLight3D& node)
{
    if (&(*GUI::m_CurrentNode_sptr) != &node)
        return;

    ImGui::Begin(node.m_Name.c_str());
    ImGui::SeparatorText(node.m_Name.c_str());
    showLight3D(node);
    ImGui::DragFloat3("Global position", glm::value_ptr(node.m_GlobalPosition));
    ImGui::DragFloat3("Direction", glm::value_ptr(node.m_Direction));
    ImGui::DragFloat("Constant", &node.m_Constant);
    ImGui::DragFloat("Linear", &node.m_Linear);
    ImGui::DragFloat("Quadratic", &node.m_Quadratic);
    ImGui::DragFloat("InnerCutoff", &node.m_InnerCutoff);
    ImGui::DragFloat("OuterCutoff", &node.m_OuterCutoff);
    ImGui::End();
}

void GuiVisitor::visit(qtzl::Billboard& node)
{
}

void GuiVisitor::visit(qtzl::SphericalBillboard& node)
{
}

void GuiVisitor::visit(qtzl::CylindricalBillboard& node)
{
}

void GuiVisitor::showNode3D(qtzl::Node3D& node)
{
    ImGui::Checkbox("Visible", &node.m_IsVisible);

    ImGui::DragFloat3("Global Position", glm::value_ptr(node.m_GlobalPosition));
    ImGui::DragFloat3("Global Rotation", glm::value_ptr(node.m_GlobalRotation));
    ImGui::DragFloat3("Relative Position", glm::value_ptr(node.m_Position));
    ImGui::DragFloat3("Relative Rotation", glm::value_ptr(node.m_Rotation));
    ImGui::DragFloat3("Scale", glm::value_ptr(node.m_Scale));
}

void GuiVisitor::showNode3D(qtzl::VisualNode3D& node)
{
    ImGui::Checkbox("Visible", &node.m_IsVisible);

    ImGui::DragFloat3("Global Position", glm::value_ptr(node.m_GlobalPosition));
    ImGui::DragFloat3("Global Rotation", glm::value_ptr(node.m_GlobalRotation));
    ImGui::DragFloat3("Relative Position", glm::value_ptr(node.m_Position));
    ImGui::DragFloat3("Relative Rotation", glm::value_ptr(node.m_Rotation));
    ImGui::DragFloat3("Scale", glm::value_ptr(node.m_Scale));

    node.updateMatrix();
}

void GuiVisitor::showLight3D(qtzl::Light3D& node)
{
    ImGui::Checkbox("Enabled", &node.m_IsEnabled);
    ImGui::DragFloat3("Color", glm::value_ptr(node.m_Color));
    ImGui::DragFloat3("Ambient", glm::value_ptr(node.m_Ambient));
    ImGui::DragFloat3("Diffuse", glm::value_ptr(node.m_Diffuse));
    ImGui::DragFloat3("Specular", glm::value_ptr(node.m_Specular));
}
