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

void creep_manager::add_spawner(Vector2d pos, int spawn_rate, int spawn_amount, std::string creep_name, double rot)
{
	if(creeps.count(creep_name) != 1)
		throw(creep_not_found(creep_name));
	Creep* new_creep = new Creep(creeps[creep_name], pos, DegToRad(rot)); 
	AddChild(new creep_spawner(this, spawn_rate, spawn_amount, new_creep));
}

void creep_manager::draw2()
{
	//The Manager itself does not need to be drawn
}

void creep_manager::add_creep_to_grid(Creep* c)
{
	Vector2d CreepPos = c->getPos();
	int i = (int)(CreepPos.x / 4);
	int j = (int)(CreepPos.y / 4);
	creep_grid[i][j].push_back(c);
}

std::list<Creep*> creep_manager::get_nearby_creep(Vector2d creep_pos, double radius)
{
	std::list<Creep*> RetVal;
	int CheckWidth = static_cast<int>(radius / 4);
	int x = static_cast<int>(creep_pos.x / 4);
	int y = static_cast<int>(creep_pos.y / 4);
	int x_min = std::max(0,x - CheckWidth);
	int x_max = std::min(GridSize - 1,x + CheckWidth);
	int y_min = std::max(0,y - CheckWidth);
	int y_max = std::min(GridSize - 1,y + CheckWidth);
	double radius_squared =  radius * radius;
	for(int i = x_min; i <= x_max; ++i)
	{
		for(int j = y_min; j <= y_max; ++j)
		{
			for (auto c : creep_grid[i][j])
			{
				if(Vector2d::distance_squared(creep_pos, c->getLocalPos()) < radius_squared)
					RetVal.push_back(c);
			}
				
		}
	}
	return RetVal;
}

UpdateResult creep_manager::update2(int ms, GlobalState &GS)
{
	for(int i = 0; i != GridSize; ++i)
	{
		for(int j = 0; j != GridSize; ++j)
		{
			creep_grid[i][j].clear();
		}
	}
	for(auto itr = Children.begin(); itr != Children.end(); ++itr)
	{
		add_creep_to_grid(static_cast<Creep*>(*itr));
	}
	return UPDATE_NONE;
}