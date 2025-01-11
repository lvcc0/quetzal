#pragma once

#include "core/gui/window.h"

#include "scene/3d/billboard.h"
#include "scene/3d/cylindrical_billboard.h"
#include "scene/3d/spherical_billboard.h"

namespace qtzl
{
    // Windows created for scene nodes only
    // NOTE: consider deleting this stuff?
    class NodeWindow : public Window
    {
    public:
        NodeWindow(std::shared_ptr<qtzl::Node> node_sptr);
        virtual ~NodeWindow() = default;

        std::shared_ptr<qtzl::Node> getNode() const;

        void render();

    private:
        std::shared_ptr<qtzl::Node> m_Node_sptr;

        template <typename T> void showNodeGUI();
        template <> void showNodeGUI<Billboard>();
        template <> void showNodeGUI<CylindricalBillboard>();
        template <> void showNodeGUI<SphericalBillboard>();
    };
}