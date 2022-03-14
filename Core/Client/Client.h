#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"

#include "Core/Types/NodeTypeList.h"
#include "Core/World/World.h"
#include "Core/Utils/Package.h"

class Client : public Singleton<Client> {

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

	void init();
	void update();

	void receivePackage(Package* package);
	void sendPackage(Package* package);

	Vector<Package*>& getPackageSendList() { return packageSendList; }
	Vector<Package*>& getPackageReceiveList() { return packageReceiveList; }

private:
	String name;
	int coreVersion;
	int modVersion;
	String modName;

	NodeTypeList* nodeTypeList;
	World* world;

	Vector<Package*> packageSendList;
	Vector<Package*> packageReceiveList;
};


#endif /* __CLIENT_H__ */
