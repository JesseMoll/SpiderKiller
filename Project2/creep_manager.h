#pragma once
#include "creep_spawner.h"
#include <map>
#include <list>
#include <vector>
#include <string>

const int GridSize = 128;
class creep_manager :
	public Drawable
{
public:
	creep_manager(Drawable* _Parent);
	~creep_manager(void);
	
	void add_spawner(Vector2d pos, int spawn_rate = 1000, int spawn_amount = 20, std::string creep_name = "", double rot = 0);
	void add_creep(std::string creep_name, double health, std::string texture_name, double scale, double speed, double turn_speed = 5, Vector3d color = Vector3d(1,1,1));

	void add_creep_to_grid(Creep* c);
	std::list<Creep*> get_nearby_creep(Vector2d creep_pos, double radius);
//Inherited Functions
	void draw2(); 
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	std::map<std::string, Creep*> creeps;
	std::vector<Creep*> creep_grid[GridSize][GridSize];
};

