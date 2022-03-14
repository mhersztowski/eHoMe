#include "Core/Server/Server.h"

void Server::init() {

	sendSendClientInfo();

}

void Server::update() {
	for (int i = 0; i < packageReceiveList.size(); i++) {
		receivePackage(packageReceiveList[i]);
		delete packageReceiveList[i];
	}

	packageReceiveList.clear();
}

void Server::receivePackage(Package* package) {

	switch (package->getId()) {
	case Package::PKG_RECEIVE_CLIENTINFO: {

	}
	case Package::PKG_RECEIVE_TYPEINFO: {

	}

	case Package::PKG_RECEIVE_WORLD: {

	}
	}
}

void Server::sendPackage(Package* package) {
	packageSendList.append(package);
}


void Server::sendSendClientInfo() {
	Package* package = new Package("master", "", Package::PKG_SEND_CLIENTINFO, NULL);
	sendPackage(package);
}
