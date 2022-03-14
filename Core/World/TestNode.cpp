#include "Core/World/TestNode.h"
#include "Core/Utils/Singleton.h"

class IntInput1Prop : public IntProperty {

    virtual void setValue(int value) {
        TestNode* tnode = (TestNode*) this->owner;
        tnode->setIntInput1(value);
    }

    virtual int getValue() {
        TestNode* tnode = (TestNode*) owner;
        return tnode->getIntInput1();
    }
};

class BoolInput1Prop : public BoolProperty {

    virtual void setValue(bool value) {
        TestNode* tnode = (TestNode*)this->owner;
        tnode->setBoolInput1(value);
    }

    virtual bool getValue() {
        TestNode* tnode = (TestNode*)owner;
        return tnode->getBoolInput1();
    }
};

class FloatInput1Prop : public FloatProperty {

    virtual void setValue(float value) {
        TestNode* tnode = (TestNode*)this->owner;
        tnode->setFloatInput1(value);
    }

    virtual float getValue() {
        TestNode* tnode = (TestNode*)owner;
        return tnode->getFloatInput1();
    }
};

class StringInput1Prop : public StringProperty {

    virtual void setValue(String& value) {
        TestNode* tnode = (TestNode*)this->owner;
        tnode->setStringInput1(value);
    }

    virtual const String& getValue() {
        TestNode* tnode = (TestNode*)owner;
        return tnode->getStringInput1();
    }
};

class NodeInput1Prop : public NodeProperty {

    virtual void setValue(Node* value) {
        TestNode* tnode = (TestNode*)this->owner;
        tnode->setNodeInput1(value);
    }

    virtual Node* getValue() {
        TestNode* tnode = (TestNode*)owner;
        return tnode->getNodeInput1();
    }
};

TestNode::TestNode() {

}

void TestNode::declareType(NodeTypeList* nodeTypeList) {
    NodeType* nodeType = nodeTypeList->defineNodeType("TestNode");

    nodeType->definePropertyType("IntInput", PropertyType::INT, PropertyType::INOUT);
}

void TestNode::defineType(NodeTypeList* nodeTypeList) {
    type = nodeTypeList->getType("TestNode");

    property_set->addPropInt("IntInput1", new IntInput1Prop());
    property_set->addPropBool("BoolInput1", new BoolInput1Prop());
    property_set->addPropFloat("FloatInput1", new FloatInput1Prop());
    property_set->addPropString("StringInput1", new StringInput1Prop());
    property_set->addPropNode("NodeInput1", new NodeInput1Prop());

}

//TestNode::


