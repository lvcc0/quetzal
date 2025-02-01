#include "node_container.h"

void NodeContainer::addNode(NODE node)
{
	m_Nodes.push_back(node);
}

void NodeContainer::performActions(NodeVisitor& visitor)
{
	for (auto node : m_Nodes)
	{
		node->accept(visitor);
	}
}
