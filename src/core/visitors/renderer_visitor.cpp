#include "renderer_visitor.h"

void RendererVisitor::visit(qtzl::StaticBody3D& node)
{
    if (node.isVisible())
        node.render(Renderer::getCurrentShaderPrograms().at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::RigidBody3D& node)
{
    if (node.isVisible())
        node.render(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::Skybox& node)
{
    if (node.isVisible()) 
    {
        if(Renderer::m_CurrentShaderPrograms.contains(ShaderProgram::SKYBOX))
        {
            Renderer::m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->activateProgram();

            Renderer::m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->setMat4("projection", Renderer::getCurrentProjectionMatrix());
            Renderer::m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX)->setMat4("view", glm::mat4(glm::mat3(Renderer::getCurrentViewMatrix())));

            node.render(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::SKYBOX));
        }
        else 
        {
            node.render(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
        }
    }
}

void RendererVisitor::visit(qtzl::PointLight3D& node)
{
    node.updateUniforms(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::DirectionalLight3D& node)
{
    node.updateUniforms(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::SpotLight3D& node)
{
    node.updateUniforms(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::CylindricalBillboard& node)
{
    if (node.isVisible())
        node.render(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}

void RendererVisitor::visit(qtzl::SphericalBillboard& node)
{
    if (node.isVisible())
        node.render(Renderer::m_CurrentShaderPrograms.at(ShaderProgram::DEFAULT));
}
