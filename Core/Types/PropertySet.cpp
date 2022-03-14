#include "Core/Types/PropertySet.h"
#include "Core/Types/Str.h"
#include "Core/Types/Node.h"

PropertySet::PropertySet(Node* owner) : owner(owner) {

}

bool PropertySet::addPropInt(const char* name, IntProperty* int_property) {

    Property *property = getPropByName(name);
    if (property)
        return false;

    NodeType* nodeType = owner->GetType();
    PropertyType* propetyType = nodeType->getPropertyType(name);
    if (propetyType == NULL)
        return false;

    if (propetyType->getType() != PropertyType::INT)
        return false;

    int_property->owner = owner;
    int_property->type = propetyType;

    prop_list.append(int_property);

    return true;
}

bool PropertySet::addPropBool(const char* name, BoolProperty* bool_property) {

    Property* property = getPropByName(name);
    if (property)
        return false;

    NodeType* nodeType = owner->GetType();
    PropertyType* propetyType = nodeType->getPropertyType(name);
    if (propetyType == NULL)
        return false;

    if (propetyType->getType() != PropertyType::BOOL)
        return false;

    bool_property->owner = owner;
    bool_property->type = propetyType;

    prop_list.append(bool_property);

    return true;
}

bool PropertySet::addPropFloat(const char* name, FloatProperty* float_property) {

    Property* property = getPropByName(name);
    if (property)
        return false;

    NodeType* nodeType = owner->GetType();
    PropertyType* propetyType = nodeType->getPropertyType(name);
    if (propetyType == NULL)
        return false;

    if (propetyType->getType() != PropertyType::FLOAT)
        return false;

    float_property->owner = owner;
    float_property->type = propetyType;

    prop_list.append(float_property);

    return true;
}

bool PropertySet::addPropString(const char* name, StringProperty* string_property) {

    Property* property = getPropByName(name);
    if (property)
        return false;

    NodeType* nodeType = owner->GetType();
    PropertyType* propetyType = nodeType->getPropertyType(name);
    if (propetyType == NULL)
        return false;

    if (propetyType->getType() != PropertyType::STRING)
        return false;

    string_property->owner = owner;
    string_property->type = propetyType;

    prop_list.append(string_property);

    return true;
}

bool PropertySet::addPropNode(const char* name, NodeProperty* node_property) {

    Property* property = getPropByName(name);
    if (property)
        return false;

    NodeType* nodeType = owner->GetType();
    PropertyType* propetyType = nodeType->getPropertyType(name);
    if (propetyType == NULL)
        return false;

    if (propetyType->getType() != PropertyType::NODE)
        return false;

    node_property->owner = owner;
    node_property->type = propetyType;

    prop_list.append(node_property);

    return true;
}

Property* PropertySet::getPropByName(const char* name) {
    String _name(name);

    for (int i =0; i< prop_list.size(); i++) {
        Property *prop = prop_list[i];
        if (prop->getType()->getName()== _name) {
           return prop;
        }
    }
    return NULL;
}

void PropertySet::createFields() {

}
