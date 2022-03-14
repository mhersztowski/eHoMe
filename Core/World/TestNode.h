#ifndef TESTNODE_H
#define TESTNODE_H

#include "Core/Types/Node.h"

class TestNode : public Node {


public:
    TestNode();

    static void declareType(NodeTypeList* nodeTypeList);
    void defineType(NodeTypeList* nodeTypeList);

    void setIntInput1(int value) { intinput1 = value; this->getPropByName("IntInput1")->setChanged(true); }
    int getIntInput1() { return intinput1; }

    void setBoolInput1(bool value) { boolinput1 = value; this->getPropByName("BoolInput1")->setChanged(true); }
    bool getBoolInput1() { return boolinput1; }

    void setFloatInput1(float value) { floatinput1 = value; this->getPropByName("FloatInput1")->setChanged(true); }
    float getFloatInput1() { return floatinput1; }

    void setStringInput1(String& value) { stringinput1 = value; this->getPropByName("StringInput1")->setChanged(true); }
    String& getStringInput1() { return stringinput1; }

    void setNodeInput1(Node* value) { node1input = value; this->getPropByName("NodeInput1")->setChanged(true); }
    Node* getNodeInput1() { return node1input; }

protected:
    int intinput1;
    bool boolinput1;
    float floatinput1;
    String stringinput1;
    Node* node1input;
};


#endif // TESTNODE_H
