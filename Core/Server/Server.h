#ifndef __SERVER_H__
#define __SERVER_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"

#include "Core/Types/NodeTypeList.h"
#include "Core/World/World.h"
#include "Core/Utils/Package.h"

class Server : public Singleton<Server> {

public:

	class Client {

	public:
		Client() {
			nodeTypeList = new NodeTypeList();
			world = new World();
		}

		const String& getName() { return name; }
		int getCoreVersion() { return coreVersion; }
		int getModVersion() { return modVersion; }
		const String& getModName() { return modName; }

		void init(const char* name, int coreVersion, int modVersion, const char* modName) {
			this->name = name; this->coreVersion = coreVersion; this->modVersion = modVersion;  this->modName = modName;
		}

		NodeTypeList* getNodeTypeList() { return nodeTypeList; }
		World* getWorld() { return world; }

	private:
		String name;
		int coreVersion;
		int modVersion;
		String modName;

		NodeTypeList* nodeTypeList;
		World* world;
	};

	void init();
	void update();

	void receivePackage(Package* package);
	void sendPackage(Package* package);

	Vector<Package*>& getPackageSendList() { return packageSendList; }
	Vector<Package*>& getPackageReceiveList() { return packageReceiveList; }

	int getNumClients() { return clientList.size();  }
	Server::Client* getClient(int index) { return clientList[index]; }


	void sendSendClientInfo();

private:
	Vector<Server::Client*> clientList;

	Vector<Package*> packageSendList;
	Vector<Package*> packageReceiveList;
};


#endif /* __SERVER_H__ */
