#pragma once

#include "core/rendering/shader_program.h"

#include "scene/3d/visual_node3d.h"

#include "scene/resources/mesh.h"
#include "scene/resources/shader.h"

namespace qtzl
{
    // 3D body that can be affected by external forces
    class RigidBody3D : public VisualNode3D
    {
    public:
        RigidBody3D(const std::string& name, std::shared_ptr<Mesh> mesh_sptr);
        virtual ~RigidBody3D() = default;

        void render(std::shared_ptr<ShaderProgram> shader_program) override;

        void translate(const glm::vec3& vector);
        void scale(const glm::vec3& vector);

        void rotate(const glm::vec3& vector);
        void rotate(float radians, const glm::vec3& vector);

        void rotateDegrees(const glm::vec3& vector);
        void rotateDegrees(float degrees, const glm::vec3& vector);

    private:
        Variant::Type m_Type = Variant::Type::RIGID_BODY3D;

        std::shared_ptr<Mesh> m_Mesh_sptr;

        void setupRender() override;
    };
}