#include "Core/Types/Node.h"
//#include "Core/Types/PropertySet.h"

/******************************************************************************\
*
* Node
*
\******************************************************************************/

/*
 */
Node::Node()  {
    property_set = new PropertySet(this);

}

Node::~Node() {
	
	// clear node hierarchy
	if(parent) parent->removeChild(this);
	for(int i = 0; i < childs.size(); i++) {
		childs[i]->parent = NULL;
	}

    delete property_set;
}

/*
 */
void Node::setName(const char *n) {
    name = calculateName(String(n));
}

const char *Node::getName() const {
    return name;
}

String& Node::calculateName(const String& def_name) {
	int num = 1;

	String cname(def_name);

	if (parent == NULL)
		return cname;

	while (parent->getChildByName(cname)) 
	{
		cname = String::format("%s%n", def_name, num);
		num++;
	}

	return cname;
}

/******************************************************************************\
*
* Hierarchy
*
\******************************************************************************/

/*
 */
void Node::setParent(Node *node) {
	assert(node != this && "Node::setParent(): it's the same node");
	if(parent == node) return;
	if(parent) parent->removeChild(this);
	if(node == NULL) return;
	parent = node;
	name = calculateName(name);
	parent->childs.append(this);
}

/*
 */
void Node::addChild(Node *node) {
	assert(node != NULL && "Node::addChild(): node is NULL");
	assert(node != this && "Node::addChild(): it's the same node");
	if(childs.find(node) != childs.end()) return;
	if(node->parent) node->parent->removeChild(node);
	node->parent = this;
	node->name = node->calculateName(node->name);
	childs.append(node);
}

/*
 */
void Node::removeChild(Node *node) {
	for(int i = 0; i < childs.size(); i++) {
		if(childs[i] == node) {
			node->parent = NULL;
			childs.remove(i);
			return;
		}
	}
}

/*
 */
int Node::isChild(const Node *node) const {
	for(int i = 0; i < childs.size(); i++) {
		if(childs[i] == node) return 1;
	}
	for(int i = 0; i < childs.size(); i++) {
		if(childs[i]->isChild(node)) return 1;
	}
	return 0;
}

int Node::getNumChilds() const {
	return childs.size();
}

Node* Node::getChild(int num) const {
	assert(num >= 0 && num < childs.size() && "Node::getChild(): bad child number");
	return childs[num];
}

Node* Node::getChildByName(const char *name) const {
	if(name == NULL) return NULL;
	for(int i = 0; i < childs.size(); i++) {
		if(!strcmp(name,childs[i]->getName())) return childs[i];
	}
	return NULL;
}

String& Node::getPath(Node* child_node) {

	Node* temp_node = child_node;
	Vector<Node*> node_path;
	node_path.append(temp_node);

	while (temp_node != this && temp_node != NULL) {
		temp_node = temp_node->getParent();
		node_path.append(temp_node);
	}

	if (temp_node == NULL)
		return String("");

	String path;

	for (int i = node_path.size() - 1; i >= 0; i--) {
		path += node_path[i]->getName();
		if (i != 0)
			path += "\\";
	}

	return path;
}

Node* Node::getNodeByPath(Node* root_node, const String& path) {
	StringArray<> path_splitted = String::split(path, "\\");

	Node* node = root_node;

	if (path_splitted.size() == 0)
		return NULL;

	if (path_splitted.size() == 1)
		if (node->getName() == path_splitted[0])
			return node;
		else
			return NULL;

	for (int i = 1; i < path_splitted.size(); i++) {
		node = node->getChildByName(path_splitted[i]);

		if (node == NULL)
			return NULL;
	}

	return node;
}


