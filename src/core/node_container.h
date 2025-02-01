#pragma once

#include <vector>

#include "scene/main/node.h"
#include "visitors/node_visitor.h"

#define NODE std::shared_ptr<qtzl::Node>

// Make function for deleting nodes 
class NodeContainer {
public:
	void addNode(NODE node);
	void performActions(NodeVisitor& visitor);

private:
	std::vector<NODE> m_Nodes;
};