#ifndef __NODE_TYPE_LIST_H__
#define __NODE_TYPE_LIST_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"

#include "Core/Types/NodeType.h"
#include "Core/Types/Node.h"

class NodeTypeList {

public:
	bool isDefined(const char* name) { return getType(name) != NULL; }

	int getNumTypes() { return nodeTypeList.size();  }
	NodeType* getType(int index) { return nodeTypeList[index]; }
	NodeType* getType(const char* name) { 
		for (int i = 0; i < nodeTypeList.size(); i++) 
		{ 
			if (nodeTypeList[i]->getName() == name)
				return nodeTypeList[i];
		} 
		return NULL;
	}

	Node* createNode(const char* nodeTypeName, Node* parent);

	NodeType* defineNodeType(const char* node_type_name) 
	{
		if (isDefined(node_type_name))
			return NULL;
		NodeType* nodeType = new NodeType(node_type_name);
		nodeTypeList.append(nodeType);
		return nodeType;
	}

	bool IsNative() { return isNative; }
	void setNative(bool native) { isNative = native; }

private:
	Vector<NodeType*> nodeTypeList;
	bool isNative;
};



#endif /* __NODE_TYPE_LIST_H__ */