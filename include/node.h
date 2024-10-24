#include <string>
#include <iostream>
#include <memory>

// Parent class for classes which objects are used as node in engine scene
class Node {
public:
	Node(std::string name);

private:
	std::string m_Name;
	std::weak_ptr<Node> m_Parent_node;

public:
	inline const std::string getName() const { return this->m_Name; }
	inline void setName(std::string name) { this->m_Name = name; }
	inline void setParent(const std::shared_ptr<Node>& parent) { this->m_Parent_node = parent; };
	inline const std::shared_ptr<Node> getParent() const { return this->m_Parent_node.lock(); };
};