#include "core/gui/node_window.h"

namespace qtzl
{
    NodeWindow::NodeWindow(std::shared_ptr<Node3D> node_sptr)
        : m_Node_sptr(node_sptr)
    {
    }

    std::shared_ptr<Node3D> NodeWindow::getNode() const
    {
        return this->m_Node_sptr;
    }

    void NodeWindow::render()
    {
        // TODO
        //if (typeid(m_Node) == typeid(/*node*/))
        //    showNodeGUI</*node*/>();
        // and so on
    }
}