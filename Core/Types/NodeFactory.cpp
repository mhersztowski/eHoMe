#include "Core/Types/NodeFactory.h"
#include "Core/World/TestNode.h"

void NodeFactory::addNodeType(const char* node_type) {
	String n_type(node_type);

	for (int i = 0; i < node_types.size(); i++) {
		if (node_types[i] > n_type) {
			node_types.append(i, n_type);
			return;
		}
	}

	node_types.append(n_type);
}


Node* NodeFactory::createNode(const char* node_type) {
	String n_type(node_type);

	if (n_type == "Test::TestNode") {
		return new TestNode();
	}

	return NULL;
}

void NodeFactory::init() {
	addNodeType("Test::TestNode");
}