#ifndef PROPERTY_H
#define PROPERTY_H

#include "Core/Types/Str.h"
//#include "Core/Types/Node.h"

#include "Core/Types/PropertyType.h"

class Node;


class Property {

public:
    PropertyType* getType() { return type; }
    bool getChanged() { return changed; }
    void setChanged(bool changed) { this->changed = changed; }
    Node* getOwner() { return owner; }

protected:
    PropertyType* type;

    bool changed;
    Node* owner;

    friend class PropertySet;
};

//typedef void (*property_int_setter)(Node* node, int value);
//typedef int (*property_int_getter)(Node* node);

class IntProperty : public Property {

public:
    virtual void setValue(int value) = 0;
    virtual int getValue() = 0;

private:
    friend class PropertySet;
};

class BoolProperty : public Property {

public:
    virtual void setValue(bool value) = 0;
    virtual bool getValue() = 0;

private:
    friend class PropertySet;
};

class FloatProperty : public Property {

public:
    virtual void setValue(float value) = 0;
    virtual float getValue() = 0;

private:
    friend class PropertySet;
};

class StringProperty : public Property {

public:
    virtual void setValue(String& value) = 0;
    virtual const String& getValue() = 0;

private:
    friend class PropertySet;
};

class NodeProperty : public Property {

public:
    virtual void setValue(Node* value) = 0;
    virtual const Node* getValue() = 0;

private:
    friend class PropertySet;
};


#endif // PROPERTY_H
