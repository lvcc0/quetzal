#include "core/gui/engine_window.h"

namespace qtzl
{
    EngineWindow::EngineWindow(EngineWindowType type)
        : m_Type(type)
    {
    }

    void EngineWindow::toggleVisibility()
    {
        this->m_Visible = !this->m_Visible;
    }

    EngineWindowType EngineWindow::getType() const
    {
        return this->m_Type;
    }

    bool EngineWindow::isVisible() const
    {
        return this->m_Visible;
    }
}