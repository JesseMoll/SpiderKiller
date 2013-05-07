#pragma once
#include "Drawable.h"
#include "Creep.h"
#include <map>
#include <string>

class creep_spawner : public Drawable
{
public:
	creep_spawner(Drawable* _Parent, int _SpawnRate = 1000, int _SpawnAmount = 20, int _SpawnLimit = 0, Creep* __CreepType = new Creep((Drawable*) (0)));
	~creep_spawner();
	void draw2(){}; // Doesn't need to be drawn
	UpdateResult update2(int ms, GlobalState &GS);
	Creep* SpawnCreep();
	void setSpawnRate(int _SpawnRate){SpawnRate = _SpawnRate;};
	void setSpawnAmount(int _SpawnAmount){SpawnAmount = _SpawnAmount;};
	void setSpawnLimit(int _SpawnLimit){SpawnTotal = 0; SpawnLimit = _SpawnLimit;};

	
protected:
	int SpawnRate;
	int SpawnAmount;
	int SpawnTimer;
	int SpawnLimit;
	int SpawnTotal;
	Creep* CreepType;
};

