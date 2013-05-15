#include "creep_spawner.h"
#include "texture_manager.h"

using namespace std;


creep_spawner::creep_spawner(Drawable* _Parent, int _SpawnRate, int _SpawnAmount, int _SpawnLimit, Creep* _CreepType, Vector2d _Pos):
	Creep(_Parent, texture_manager::get_texture_name("Spawn.bmp"), _Pos, /*HEALTH*/ 500, 7,0,0,0,/*COLOR*/Vector4d(.5,0,.5,1)),
	SpawnRate(_SpawnRate),
	SpawnAmount(_SpawnAmount),
	SpawnTimer(_SpawnRate),
	SpawnLimit(_SpawnLimit),
	SpawnTotal(0),
	CreepType(_CreepType)
{
}

UpdateResult creep_spawner::update2(int ms, GlobalState &GS)
{
	//It's dead
	if(Health == 0)
	{
		OnDeath(GS);
		return UPDATE_DELETE;
	}
	SpawnTimer -= ms;
	if(SpawnTimer < 0 && Parent->Children.size() < MaxCreep)
	{
		SpawnTimer += SpawnRate;
		for(int n = 0; n != SpawnAmount; n++)
		{
			SpawnTotal++;
			if (SpawnLimit != 0 && SpawnTotal > SpawnLimit)
				return UPDATE_DELETE;
			GS.TheCreepManager->AddChild(SpawnCreep());
		}
	}
	return UPDATE_NONE;
}

creep_spawner::~creep_spawner(void)
{
}

Creep* creep_spawner::SpawnCreep()
{	
	Creep* NewCreep = CreepType->clone();
	NewCreep->setPos(NewCreep->getPos() + Vector2d(Random(10.0) - 5.0, Random(10.0) - 5.0)); 
	NewCreep->setRot(NewCreep->getRot() + Random(.2) - .1);
	return NewCreep;
}

//Damage Function Is contained within Creep Class
