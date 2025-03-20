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
        void setScale(const glm::vec3& scale);

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& radians);
        void setRotationDegrees(const glm::vec3& degrees);

        void setGlobalPosition(const glm::vec3& position);
        void setGlobalRotation(const glm::vec3& radians);
        void setGlobalRotationDegrees(const glm::vec3& degrees);

        virtual glm::mat4 getModelMatrix() const;

        void render(const std::shared_ptr<ShaderProgram>& shader_program) {};

        void accept(NodeVisitor& visitor) override;

        void updateMatrix();
    protected:
        glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

        virtual void setupRender() = 0;
    };
}