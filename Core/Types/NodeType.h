#ifndef __NODE_TYPE_H__
#define __NODE_TYPE_H__

#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"
#include "Core/Types/PropertyType.h"

class NodeTypeUtils;
class Node;

class NodeType {

public:
	NodeType(const char* name) { this->name = name; }

	const String& getName() { return name; }
	bool IsNative() { return isNative; }

	int getNumPropertyTypes() { return propertyTypeList.size(); }
	PropertyType* getPropertyType(int index) { return propertyTypeList[index]; }

	PropertyType* getPropertyType(const char* name) {
		for (int i = 0; i < propertyTypeList.size(); i++)
		{
			if (propertyTypeList[i]->getName() == name)
				return propertyTypeList[i];
		}
		return NULL;
	}

	PropertyType* definePropertyType(const char* name, PropertyType::Kind type, PropertyType::Direction dir) 
	{ 
		PropertyType* propertyType = getPropertyType(name);
		if (propertyType != NULL)
			return NULL;

		propertyType = new PropertyType(name, type, dir);
		propertyTypeList.append(propertyType);
		return propertyType;
	}

private:
	String name;
	bool isNative;

	Vector<PropertyType*> propertyTypeList;
};

class NodeTypeUtils {
public:
	virtual Node* createNode() = 0;

};



#endif /* __NODE_TYPE_H__ */