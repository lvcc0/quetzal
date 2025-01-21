#pragma once

#include "core/rendering/shader_program.h"

// TODO: change map key on set("Name")

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

        void addChild(std::shared_ptr<Node> node);
        void removeChild(const std::string& name);

    protected:
        std::shared_ptr<Node> m_Parent;
        std::map<std::string, std::shared_ptr<Node>> m_Children;
	};
}