#pragma once
#include "Creep.h"
#include <map>
#include <string>

//Lets make it a creep so damage/projectile collision is easy
class creep_spawner : public Creep
{
public:
	creep_spawner(Drawable* _Parent, int _SpawnRate = 1000, int _SpawnAmount = 20, int _SpawnLimit = 0, Creep* __CreepType = new Creep((Drawable*) (0)), Vector2d _Pos = Vector2d(0, 0));
	~creep_spawner();
	UpdateResult update2(int ms, GlobalState &GS);
	Creep* SpawnCreep();
	void setSpawnRate(int _SpawnRate){SpawnRate = _SpawnRate;};
	void setSpawnAmount(int _SpawnAmount){SpawnAmount = _SpawnAmount;};
	void setSpawnLimit(int _SpawnLimit){SpawnTotal = 0; SpawnLimit = _SpawnLimit;};
	//Damage() and health are both in creep class
	
protected:
	int SpawnRate;
	int SpawnAmount;
	int SpawnTimer;
	int SpawnLimit;
	int SpawnTotal;
	Creep* CreepType;
};

