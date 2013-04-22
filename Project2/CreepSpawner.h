#pragma once
#include "Drawable.h"
#include "Creep.h"
#include <map>
#include <string>

class CreepSpawn : public Drawable
{
public:
	CreepSpawn(int _SpawnRate = 1000, Creep* __CreepType = new Creep((Drawable*) (0)));
	Creep* SpawnCreep();
	Creep* CheckSpawnTimer(int ms, GlobalState &GS);
protected:
	int SpawnRate;
	int SpawnTimer;
	Creep* CreepType;
};

class CreepSpawner : public Drawable
{
public:
	CreepSpawner(Drawable* _Parent);
	~CreepSpawner(void);
	
	void AddSpawner(std::string CreepName, Vector2d _Pos, int _SpawnRate = 1000, GLuint _Texture = 0, double _Scale = .5, double _Speed = .25, double _TurnSpeed = 5, Vector3d _Color = Vector3d (1,1,1));
	

//Inherited Functions
	void draw2(); 
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	std::map<std::string, CreepSpawn>  Spawns; 
};

