#ifndef __WORLD_H__
#define __WORLD_H__

#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"
#include "Core/Utils/Singleton.h"

class Node;

class World  {

	public:

		Node* getRootNode() { return root_node; }
		void setRootNode(Node* node) { root_node = node; }
	protected:
		Node* root_node;
};

#endif /* __WORLD_H__ */
