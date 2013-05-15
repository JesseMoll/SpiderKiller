#include "creep_manager.h"
#include "texture_manager.h"
#include "exceptions.h"


creep_manager::creep_manager(Drawable* _Parent):Drawable(_Parent)
{

}

creep_manager::~creep_manager(void)
{
}

Creep* creep_manager::get_creep(std::string creep_name)
{
	return creeps[creep_name];
}

Creep* creep_manager::add_creep(std::string creep_name, double health, std::string texture_name, double scale, double speed, double turnSpeed, Vector3d color,std::string _SpawnOnDeath, std::string _SpawnPeriodic, double _PeriodicSpawnRate, double _PeriodicSpawnNum, double _OnDeathSpawnNum)
{
	Creep* SpawnOnDeath = NULL;
	Creep* SpawnPeriodic = NULL;
	if (_SpawnOnDeath != "")
		SpawnOnDeath = creeps[_SpawnOnDeath];
	if (_SpawnPeriodic != "")
		SpawnPeriodic = creeps[_SpawnPeriodic];
	return creeps[creep_name] = new Creep(this,texture_manager::get_texture_name(texture_name), Vector2d(0,0), health, scale, 0, speed, turnSpeed, color,SpawnOnDeath, SpawnPeriodic, _PeriodicSpawnRate, _PeriodicSpawnNum, _OnDeathSpawnNum);
}

Creep* creep_manager::add_creep(std::string creep_name, Creep* NewCreep)
{
	return creeps[creep_name] = NewCreep;
}

creep_spawner* creep_manager::add_spawner(Vector2d pos, int spawn_rate, int spawn_amount, int spawn_limit, std::string creep_name, double rot)
{
	creep_spawner* RetVal = 0;
	if(creeps.count(creep_name) != 1)
		throw(creep_not_found(creep_name));
	Creep* new_creep = creeps[creep_name]->clone();
	new_creep->setPos(pos);
	new_creep->setRot(DegToRad(rot));
	//Add the spawners to the same place as the rest of the creep
	//only problem I see is running into the spawn with the hero will most likely kill him (so don't run into them)
	AddChild(RetVal = new creep_spawner(this, spawn_rate, spawn_amount, spawn_limit, new_creep, pos));
	return RetVal;
}

void creep_manager::draw2()
{
	//The Manager itself does not need to be drawn
}





UpdateResult creep_manager::update2(int ms, GlobalState &GS)
{
	GS.TheGrid->UpdateGrid(&Children);
	return UPDATE_NONE;
}