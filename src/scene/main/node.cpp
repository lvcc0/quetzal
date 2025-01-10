#include "scene/main/node.h"

namespace qtzl
{
	Node::Node(const std::string& name)
		: m_Name(name)
	{
	}

	void Node::setName(const std::string& name)
	{
		this->m_Name = name;
	}

	void Node::setParent(std::shared_ptr<Node> node)
	{
		this->m_Parent = node;
	}

	std::string Node::getName() const
	{
		return this->m_Name;
	}

	std::shared_ptr<Node> Node::getParent() const
	{
		return this->m_Parent;
	}

	std::map<std::string, std::shared_ptr<Node>> Node::getChildren() const
	{
		return this->m_Children;
	}

	bool Node::isRenderable() const
	{
		return this->m_Renderable;
	}

	void Node::addChild(std::shared_ptr<Node> node)
	{
		node->setParent(shared_from_this()); // NOTE: yeah this doesn't seem right
		this->m_Children.emplace(node->getName(), node);
	}

	void Node::removeChild(const std::string& name)
	{
		this->m_Children.erase(name);
	}
}