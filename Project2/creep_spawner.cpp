#include "creep_spawner.h"
#include "texture_manager.h"

using namespace std;


creep_spawner::creep_spawner(Drawable* _Parent, int _SpawnRate, int _SpawnAmount, int _SpawnLimit, Creep* _CreepType):
	Drawable(_Parent),
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
