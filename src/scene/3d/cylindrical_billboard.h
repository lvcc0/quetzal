#pragma once

#include "scene/3d/billboard.h"

namespace qtzl
{
    // A flat plane rotating around Y-axis relatively to the camera
    class CylindricalBillboard : public Billboard
    {
    public:
        CylindricalBillboard(const std::string& name, std::shared_ptr<Texture> texture);
        virtual ~CylindricalBillboard() = default;

        void draw(const ShaderProgram& shader_program) override;
    };
}