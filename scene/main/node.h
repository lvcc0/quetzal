#pragma once

// std
#include <string>
#include <memory>
#include <map>

namespace qtzl
{
    // Base class for every single scene object
	class Node : public std::enable_shared_from_this<Node>
	{
    public:
        Node(const std::string& name);
        virtual ~Node() = default;

        void setName(const std::string& name);
        void setParent(std::shared_ptr<Node> node);

        std::string                                  getName() const;
        std::shared_ptr<Node>                        getParent() const;
        std::map<std::string, std::shared_ptr<Node>> getChildren() const;

        void addChild(std::shared_ptr<Node> node);
        void removeChild(const std::string& name);

    protected:
        std::string m_Name;
        std::shared_ptr<Node> m_Parent;
        std::map<std::string, std::shared_ptr<Node>> m_Children;
	};
}