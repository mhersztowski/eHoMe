#include "Core/Client/Client.h"

#include "Core/World/TestNode.h"

void Client::init() {
	Client::get()->init("AlarmClockSound", 1, 1, "ModSound");

	// Define Nodes
	NodeTypeList* nodeTypeList = Client::get()->getNodeTypeList();
	nodeTypeList->setNative(true);

	TestNode::declareType(nodeTypeList);


	// Create World

	Node* client_node = nodeTypeList->createNode("Client", NULL);

	Client::get()->getWorld()->setRootNode(client_node);

	Node* test_node = nodeTypeList->createNode("TestNode", client_node);
}

void Client::update() {

	for (int i = 0; i < packageReceiveList.size(); i++) {
		receivePackage(packageReceiveList[i]);
		delete packageReceiveList[i];
	}

	packageReceiveList.clear();
}

void Client::receivePackage(Package* package) {

	switch (package->getId()) {
	case Package::PKG_SEND_CLIENTINFO: {

	}
	case Package::PKG_SEND_TYPEINFO: {

	}

	case Package::PKG_SEND_WORLD: {

	}
	}
}

void Client::sendPackage(Package* package) {
	packageSendList.append(package);
}
