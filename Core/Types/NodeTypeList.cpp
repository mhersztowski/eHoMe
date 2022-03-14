#include "Core/Types/NodeTypeList.h"

Node* NodeTypeList::createNode(const char* nodeTypeName, Node* parent) {

	NodeType* nodeType = getType(nodeTypeName);

	if (!nodeType)
		return NULL;

	Node* node = NULL;

	if (IsNative()) {
		
	}
	else {
		node = new Node();
		node->getPropertySet()->createFields();
	}

	node->setParent(parent);

	return node;
}