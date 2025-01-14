#pragma once

#include "core/gui/window.h"

namespace qtzl
{
    enum class EngineWindowType { SceneCfg, NodeMgr, ResourceMgr };

    // NOTE: maybe we'll add some more stuff to this class
    // Windows containing all the tools and such
    class EngineWindow : public Window
    {
    public:
        EngineWindow(EngineWindowType type);
        virtual ~EngineWindow() = default;

        void toggleVisibility();

        EngineWindowType getType() const;
        bool isVisible() const;

    private:
        EngineWindowType m_Type;
        bool m_Visible = true;
    };
}