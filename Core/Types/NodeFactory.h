#ifndef __NODE_FACTORY_H__
#define __NODE_FACTORY_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"

class Node;

class NodeFactory : public Singleton<NodeFactory> {

public:
	int getNumNodeTypes() { return node_types.size(); }
	const String& getNodeType(int index) { return node_types[index];  }
	void addNodeType(const char* node_type);


	Node* createNode(const char* node_type);
	void init();

private:
	Vector<String> node_types;
};


#endif /* __NODE_FACTORY_H__ */