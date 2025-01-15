#pragma once

#include "scene/3d/light3d.h"

namespace qtzl
{
    // Light emitting in a specific direction
    class SpotLight3D : public Light3D
    {
    public:
        SpotLight3D(
            const std::string& name,
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 direction = glm::vec3(0.0f),
            glm::vec3 color = glm::vec3(1.0f),
            glm::vec3 ambient = glm::vec3(0.5f),
            glm::vec3 diffuse = glm::vec3(0.5f),
            glm::vec3 specular = glm::vec3(0.5f),
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            float inner_cutoff = 7.5f,
            float outer_cutoff = 10.0f
        );
        virtual ~SpotLight3D() = default;

        void setBillboardEnabled(bool enabled);

        void enableBillboard();
        void disableBillboard();

        void updateUniforms(const std::shared_ptr<ShaderProgram>& shader_program, int index) const override;

        void render(std::shared_ptr<ShaderProgram> shader_program) override;

    private:
        Variant::Type m_Type = Variant::Type::SPOT_LIGHT3D;

        bool m_BillboardEnabled = true;

        glm::vec3 m_Position;
        glm::vec3 m_Direction;

        float m_Constant;
        float m_Linear;
        float m_Quadratic;

        float m_InnerCutoff;
        float m_OuterCutoff;
    };
}