#include "core/gui/window.h"

namespace qtzl
{
    Window::Window()
    {
    }

    glm::vec2 Window::getPosition() const
    {
        return this->m_Position;
    }

    glm::vec2 Window::getSize() const
    {
        return this->m_Size;
    }
}