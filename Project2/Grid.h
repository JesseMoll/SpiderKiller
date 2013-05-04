#pragma once
#include <list>
#include "Cell.h"
#include "Drawable.h"

const int LevelSize = 512;

const int GridSize = LevelSize / CellSize;


class Grid: 
	public Drawable
{
public:
	Grid(void);
	~Grid(void);
	
	void UpdateGrid(std::list<Drawable*>* ChildList);
	std::list<Cell*> get_nearby_cells(Vector2d creep_pos, double radius);
	void draw2();
	Cell* GetCell(int x, int y);
	Cell* GetCell(Vector2d &Pos);
	void UpdatePathing(Cell* Target);
protected:
	void add_creep(Drawable* c);
	UpdateResult update2(int ms, GlobalState &GS);
	Cell cells[GridSize][GridSize];
};

