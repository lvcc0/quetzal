#pragma once

#include "core/rendering/shader_program.h"

namespace qtzl
{
    // Base class for every single scene object
	class Node : public Object, public std::enable_shared_from_this<Node>
	{
    public:
        Node(const std::string& name);
        virtual ~Node() = default;

        void setParent(std::shared_ptr<Node> node);

        std::shared_ptr<Node>                        getParent() const;
        std::map<std::string, std::shared_ptr<Node>> getChildren() const;

        bool isRenderable() const;

        void addChild(std::shared_ptr<Node> node);
        void removeChild(const std::string& name);

        virtual void render(std::shared_ptr<ShaderProgram> shader_program) = 0;

    protected:
        std::shared_ptr<Node> m_Parent;
        std::map<std::string, std::shared_ptr<Node>> m_Children;

        bool m_Renderable = false;
	};
}