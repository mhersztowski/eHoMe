#ifndef __WORLDS_H__
#define __WORLDS_H__

#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"
#include "Core/Utils/Singleton.h"

class World;

class Worlds : public Singleton<Worlds> {

public:
	int getNumWorlds() { return worlds.size(); }
	World* getWorld(int index) { return worlds[index]; }

	void init();

private:
	Vector<World*> worlds;
};

#endif /* __WORLDS_H__ */
