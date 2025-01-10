#pragma once

// thirdparty
#include <glm/glm.hpp>

namespace qtzl
{
    // Base window class
    class Window
    {
    public:
        Window();
        Window(const Window&) = delete;
        Window(Window&&) = delete;

        virtual ~Window() = default;

        Window& operator= (const Window&) = delete;

        inline glm::vec2 getPosition() const;
        inline glm::vec2 getSize() const;

    protected:
        glm::vec2 m_Position;
        glm::vec2 m_Size;
    };
}