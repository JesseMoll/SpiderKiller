#include "creep_manager.h"
#include "texture_manager.h"
#include "exceptions.h"


creep_manager::creep_manager(Drawable* _Parent):Drawable(_Parent)
{

}

creep_manager::~creep_manager(void)
{
}

void creep_manager::add_creep(std::string creep_name, double health, std::string texture_name, double scale, double speed, double turnSpeed, Vector3d color)
{
	creeps[creep_name] = new Creep(this,texture_manager::get_texture_name(texture_name), Vector2d(0,0), health, scale, 0, speed, turnSpeed, color);
}

creep_spawner* creep_manager::add_spawner(Vector2d pos, int spawn_rate, int spawn_amount, int spawn_limit, std::string creep_name, double rot)
{
	creep_spawner* RetVal = 0;
	if(creeps.count(creep_name) != 1)
		throw(creep_not_found(creep_name));
	Creep* new_creep = new Creep(creeps[creep_name], pos, DegToRad(rot)); 
	AddChild(RetVal = new creep_spawner(this, spawn_rate, spawn_amount, spawn_limit, new_creep));
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