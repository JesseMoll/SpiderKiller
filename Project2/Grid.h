#pragma once
#include <list>
#include "Cell.h"
#include "Drawable.h"




class Grid: 
	public Drawable
{
public:
	Grid(void);
	~Grid(void);
	void Init();
	void UpdateGrid(std::list<Drawable*>* ChildList);
	void get_nearby_cells(std::list<Cell*> &CellList, const Vector2d &creep_pos, const double &radius);
	void draw2(){};
	void draw();
	Cell* GetCell(int x, int y);
	Cell* GetCell(Vector2d &Pos);
	void UpdatePathing(Cell* Target);
protected:
	void add_creep(Drawable* c);
	void Addconnections(int i, int j, int iAdder, int jAdder);
	UpdateResult update2(int ms, GlobalState &GS);
	std::vector< std::vector<Cell> > cells;
	//Cell cells[GridSize][GridSize];
	bool debug;
	
};

