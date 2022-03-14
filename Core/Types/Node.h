#ifndef __NODE_H__
#define __NODE_H__

#include "Str.h"
#include "Vector.h"
#include "Core/Types/PropertySet.h"
#include "Core/Types/NodeTypeList.h"


/*
 */
class Node;
class PropertySet;

/*
 */
class Node {
		
	public:
		
        Node();
		virtual ~Node();
		
		// name
		void setName(const char *name);
		const char *getName() const;
		
        NodeType* GetType() { return type; }

		// parent
		void setParent(Node *node);
		INLINE Node *getParent() const { return parent; }
		
		// childs
		void addChild(Node *node);
		void removeChild(Node *node);
		
		int isChild(const Node *node) const;
		int getNumChilds() const;
		Node* getChild(int num) const;
        Node* getChildByName(const char *name) const;
		
		
        Property* getPropByName(const char* name) { return property_set->getPropByName(name); }

        PropertySet* getPropertySet() { return property_set;}

		String& getPath(Node* child_node);
		static Node* getNodeByPath(Node* root_node, const String& path);

    protected:
        PropertySet* property_set;
		NodeType* type;

		String& calculateName(const String& def_name);

		//bool defineType();

	private:
		
        String name;						// node name

		Node *parent;							// parent node
		Vector<Node*> childs;					// child nodes
};

#endif /* __NODE_H__ */
