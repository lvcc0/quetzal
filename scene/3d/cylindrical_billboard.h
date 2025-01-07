#pragma once

#include "scene/3d/billboard.h"

namespace qtzl
{
    // A flat plane rotating around Y-axis relatively to the camera
    class CylindricalBillboard : public Billboard
    {
    public:
        CylindricalBillboard(const std::string& name, std::shared_ptr<Texture>& texture);
        virtual ~CylindricalBillboard() = default;

        glm::mat4 getModelMatrix() const override;

        // Draw billboard and change it's model matrix
        void draw(const Shader& shader) override;
    };
}