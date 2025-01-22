#pragma once

#include "scene/3d/billboard.h"

// TODO: fix the math with rotation and scaling

namespace qtzl
{
    // A flat plane rotating around Y-axis relatively to the camera
    class CylindricalBillboard : public Billboard
    {
    public:
        CylindricalBillboard(const std::string& name, std::shared_ptr<Texture> texture);
        virtual ~CylindricalBillboard() = default;

        void render(const std::shared_ptr<ShaderProgram>& shader_program) override;
    };
}