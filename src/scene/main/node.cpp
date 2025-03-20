#include "scene/main/node.h"

namespace qtzl
{
    Node::Node(const std::string& name)
        : Object(Object::Type::NODE), m_Name(name)
    {
    }

    void Node::setParent(std::shared_ptr<Node> node)
    {
        this->m_Parent = node;
    }

    std::shared_ptr<Node> Node::getParent() const
    {
        return this->m_Parent;
    }

    const NodeContainer& Node::getChildren() const
    {
        return this->m_Children;
    }

    void Node::addChild(std::shared_ptr<Node> node)
    {
        node->setParent(shared_from_this()); // NOTE: yeah this doesn't seem right
        m_Children.addNode(node);
    }

    void Node::removeChild(const std::string& name)
    {
        m_Children.deleteNode(name);
    }
}

void NodeContainer::addNode(std::shared_ptr<qtzl::Node> node)
{
    m_Nodes.push_back(node);
}

void NodeContainer::deleteNode(const std::string name)
{
    auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [name](std::shared_ptr<qtzl::Node> obj) {return obj->m_Name == name; });
    if(it != m_Nodes.end())
        m_Nodes.erase(it);
}

void NodeContainer::performActions(NodeVisitor& visitor) const
{
    for (auto node : m_Nodes)
        node->accept(visitor);
}

const std::vector<std::shared_ptr<qtzl::Node>>& NodeContainer::getNodes() const
{
    return m_Nodes;
}
