#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include "Core/Types/Vector.h"
#include "Core/Types/Property.h"

class Node;

class PropertySet {

public:
    PropertySet(Node* owner);

    bool addPropInt(const char* name, IntProperty* iproperty_int);
    bool addPropBool(const char* name, BoolProperty* iproperty_bool);
    bool addPropFloat(const char* name, FloatProperty* iproperty_float);
    bool addPropString(const char* name, StringProperty* iproperty_string);
    bool addPropNode(const char* name, NodeProperty* iproperty_node);

    Property* getPropByName(const char* name);
    Property* getProp(int index) { return prop_list[index]; }
    int getNumProp() { return prop_list.size(); }

    void createFields();

private:
    Vector<Property*> prop_list;
    Node* owner;
};

#endif // PROPERTYSET_H
