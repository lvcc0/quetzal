#pragma once

#include "core/rendering/vertex_array.h"

#include "scene/3d/node3d.h"

namespace qtzl
{
    // Base class for every 3D node that can be drawn on the scene
    class VisualNode3D : public Node3D
    {
    public:
        VisualNode3D(const std::string& name);
        virtual ~VisualNode3D() = default;

        //void set(const std::string& property_name, const glm::vec3& value);
        void setScale(const glm::vec3& scale)  override;

        void setPosition(const glm::vec3& position) override;
        void setRotation(const glm::vec3& radians)  override;
        void setRotationDegrees(const glm::vec3& degrees)  override;

        void setGlobalPosition(const glm::vec3& position)  override;
        void setGlobalRotation(const glm::vec3& radians)  override;
        void setGlobalRotationDegrees(const glm::vec3& degrees)  override;

        void scale(const glm::vec3& scale)  override;
        void translate(const glm::vec3& position)  override;
        void rotate(const glm::vec3& radians)  override;
        void rotateDegrees(const glm::vec3& degrees)  override;

        virtual glm::mat4 getModelMatrix() const;

        void render(const std::shared_ptr<ShaderProgram>& shader_program) {};

        void accept(NodeVisitor& visitor) override;

        void updateMatrix();
    protected:
        glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

        virtual void setupRender() = 0;
    };
}