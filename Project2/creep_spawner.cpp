#include "creep_spawner.h"
#include "texture_manager.h"
#include "creep_manager.h"
#include <algorithm>

using namespace std;


creep_spawner::creep_spawner(Drawable* _Parent, int _SpawnRate, int _SpawnAmount, int _SpawnLimit, Creep* _CreepType, Vector2d _Pos):
	Drawable(_Parent, texture_manager::get_texture_name("Spawn.bmp"), _Pos, Vector2d(7, 7)),
	SpawnRate(_SpawnRate),
	SpawnAmount(_SpawnAmount),
	SpawnTimer(_SpawnRate),
	SpawnLimit(_SpawnLimit),
	SpawnTotal(0),
	Health(1),
	CreepType(_CreepType)
{
}

UpdateResult creep_spawner::update2(int ms, GlobalState &GS)
{
	if (Health == 0)
		return UPDATE_DELETE;
	
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

double creep_spawner::Damage(double dmg)
{
	//Returns the overkill damage (projectiles will pass through if they have more damage)
	double RetVal = 0;
	Health -= dmg;
	if(Health < 0)
	{
		RetVal = -Health;
		Health = 0;
	}
	return RetVal;
}