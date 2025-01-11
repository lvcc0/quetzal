#include "core/gui/node_window.h"

namespace qtzl
{
    NodeWindow::NodeWindow(std::shared_ptr<Node> node_sptr)
        : m_Node_sptr(node_sptr)
    {
    }

    std::shared_ptr<Node> NodeWindow::getNode() const
    {
        return this->m_Node_sptr;
    }

    void NodeWindow::render()
    {
        // NOTE: gotta rewrite this
        if (typeid(*m_Node_sptr) == typeid(Billboard))
            this->showNodeGUI<Billboard>();
        else if (typeid(*m_Node_sptr) == typeid(CylindricalBillboard))
            this->showNodeGUI<CylindricalBillboard>();
        else if (typeid(*m_Node_sptr) == typeid(SphericalBillboard))
            this->showNodeGUI<SphericalBillboard>();
    }

    template <typename T>
    void NodeWindow::showNodeGUI()
    {
    }

    template <>
    void NodeWindow::showNodeGUI<Billboard>()
    {
    }

    template <>
    void NodeWindow::showNodeGUI<CylindricalBillboard>()
    {
    }

    template <>
    void NodeWindow::showNodeGUI<SphericalBillboard>()
    {
    }
}