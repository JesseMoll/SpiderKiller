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
	AddSpawner("s1", Vector2d(256,300), 4000, 0, .5, 15, 270, Vector3d(0,0,1), 270);
	AddSpawner("s2", Vector2d(256,200), 5000, 0, .25, 19, 360, Vector3d(0,1,1), 90);
//	AddSpawner("s3", Vector2d(156,256), 500, 0, .5, .25, 6, Vector3d(1,0,0));
//	AddSpawner("s4", Vector2d(356,256), 1000, 0, .9, .15, 4, Vector3d(1,0,1));
}


CreepSpawner::~CreepSpawner(void)
{
}

Creep* CreepSpawn::SpawnCreep()
{	
	Creep* NewCreep = new Creep(CreepType);
	NewCreep->setPos(NewCreep->getPos() + Vector2d(Random(10.0) - 5.0, Random(10.0) - 5.0)); 
	NewCreep->setRot(NewCreep->getRot() + Random(10.0) - 5.0);
	return NewCreep;
}

void CreepSpawner::AddSpawner(std::string CreepName, Vector2d _Pos, int _SpawnRate, GLuint _Texture, double _Scale, double _Speed, double _TurnSpeed, Vector3d _Color, double _Rot)
{
	Spawns[CreepName] = CreepSpawn(_SpawnRate, new Creep(this,_Texture,_Pos,_Scale, _Rot, _Speed, _TurnSpeed, _Color));
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
				for(int n = 0; n != 25; n++)
					AddChild(i->second.SpawnCreep());
			if(i->first == "s2")
				for(int n = 0; n != 50; n++)
					AddChild(i->second.SpawnCreep());
		}
	}

	return UPDATE_NONE;
}