#pragma once

#include <vector>

#include "core/rendering/shader_program.h"
#include "core/visitors/node_visitor.h"

// thirdparty
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: change map key on set("Name")

// Maybe we have to use weak ptr for parents and children (otherwise objects ptrs dont delete objects)
namespace qtzl {
    class Node;
}

class NodeContainer {
public:
    void addNode(std::shared_ptr<qtzl::Node> node);
    void deleteNode(const std::string name);

    void performActions(NodeVisitor& visitor) const;

    const std::vector<std::shared_ptr<qtzl::Node>>& getNodes() const;
private:
    std::vector<std::shared_ptr<qtzl::Node>> m_Nodes;
};

namespace qtzl
{
    // Base class for every single scene object
    class Node : public Object, public std::enable_shared_from_this<Node>
    {
    public:
        Node(const std::string& name);
        virtual ~Node() = default;

        void setParent(std::shared_ptr<Node> node);

        std::shared_ptr<Node> getParent() const;
        const NodeContainer& getChildren() const;

        void addChild(std::shared_ptr<Node> node);
        void removeChild(const std::string& name);

        // Accepting visitors
        virtual void accept(NodeVisitor& visitor) = 0;

        std::string m_Name;
    protected:
        std::shared_ptr<Node> m_Parent;
        NodeContainer m_Children;
    };
}