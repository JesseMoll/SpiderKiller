#include "CreepSpawner.h"

using namespace std;



CreepSpawn::CreepSpawn(int _SpawnRate, Creep* _CreepType):
	SpawnRate(_SpawnRate),
	SpawnTimer(_SpawnRate),
	CreepType(_CreepType)
{
}

Creep* CreepSpawn::CheckSpawnTimer(int ms, GlobalState &GS)
{
	SpawnTimer -= ms;
	if(SpawnTimer < 0)
	{
		SpawnTimer += SpawnRate;
		return SpawnCreep();
	}
	return 0;
}

CreepSpawner::CreepSpawner(Drawable* _Parent):Drawable(_Parent)
{
	AddSpawner("s1", Vector2d(256,356), 2000, 0, .5, .25, 6, Vector3d(0,0,1));
	AddSpawner("s2", Vector2d(256,156), 4000, 0, 1.5, .1, 3, Vector3d(0,1,1));
	AddSpawner("s3", Vector2d(156,256), 500, 0, .5, .25, 6, Vector3d(1,0,0));
//	AddSpawner("s4", Vector2d(356,256), 1000, 0, .9, .15, 4, Vector3d(1,0,1));
}


CreepSpawner::~CreepSpawner(void)
{
}

Creep* CreepSpawn::SpawnCreep()
{	
	Creep* NewCreep = new Creep(CreepType);
	NewCreep->setPos(NewCreep->getPos() + Vector2d(Random() * 5, Random() * 5)); 

	return NewCreep;
}

void CreepSpawner::AddSpawner(std::string CreepName, Vector2d _Pos, int _SpawnRate, GLuint _Texture, double _Scale, double _Speed, double _TurnSpeed, Vector3d _Color)
{
	Spawns[CreepName] = CreepSpawn(_SpawnRate, new Creep(this,_Texture,_Pos,_Scale, _Speed, _TurnSpeed, _Color));
}

void CreepSpawner::draw2()
{
	//The Spawner itself does not need to be drawn
}

UpdateResult CreepSpawner::update2(int ms, GlobalState &GS)
{
	for(std::map<std::string, CreepSpawn>::iterator i = Spawns.begin(); i != Spawns.end(); i++)
	{
		Creep* CreepToAdd = i->second.CheckSpawnTimer(ms, GS);
		if(CreepToAdd)
		{
			AddChild(CreepToAdd);
			if(i->first == "s1")
				for(int n = 0; n != 20; n++)
					AddChild(i->second.SpawnCreep());
		}
	}

	return UPDATE_NONE;
}