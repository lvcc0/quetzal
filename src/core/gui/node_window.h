#pragma once

#include "core/gui/window.h"
#include "scene/3d/node3d.h"

namespace qtzl
{
    // Windows created for scene nodes only
    class NodeWindow : public Window
    {
    public:
        NodeWindow(std::shared_ptr<qtzl::Node3D> node_sptr);
        virtual ~NodeWindow() = default;

        std::shared_ptr<qtzl::Node3D> getNode() const;

        void render();

    private:
        std::shared_ptr<qtzl::Node3D> m_Node_sptr;

        // TODO: yeah
        //template <typename T> void showNodeGUI();
        //template <typename T> void showNodeGUI</*node*/>();
        //template <typename T> void showNodeGUI</*node*/>();
    };
}