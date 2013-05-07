#pragma once
#include "creep_spawner.h"
#include "Grid.h"
#include <map>

#include <vector>
#include <string>


class creep_manager :
	public Drawable
{
public:
	creep_manager(Drawable* _Parent);
	~creep_manager(void);
	
	creep_spawner* add_spawner(Vector2d pos, int spawn_rate = 1000, int spawn_amount = 20, int spawn_limit = 0, std::string creep_name = "", double rot = 0);
	void add_creep(std::string creep_name, double health, std::string texture_name, double scale, double speed, double turn_speed = 5, Vector3d color = Vector3d(1,0,0));

	void add_creep_to_grid(Creep* c);
	
//Inherited Functions
	void draw2(); 
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	
	std::map<std::string, Creep*> creeps;
	
};

