#include "Cell.h"
#include <GL/glut.h>
#include "texture_manager.h"



Cell::Cell(void)
{
	Texture = texture_manager::get_texture_name("Arrow.bmp");
	Scale = Vector2d(CellSize * .3, CellSize * .3);
}


Cell::~Cell(void)
{
}

void Cell::AddConnection(Cell* ConnectedCell, double Distance)
{
	
	if(ConnectedCell != NULL)
	{
		if(ConnectedCell->UnwalkableWeight != MaxWeight)
			Connections.push_back(Node(this, ConnectedCell, Distance * (MaxWeight / (MaxWeight - ConnectedCell->UnwalkableWeight))));
	}
}

UpdateResult Cell::update2(int ms, GlobalState &GS)
{
	if(TargetCell)
	{
		Vector2d PathingDirection = TargetCell->Pos - Pos;
		Rot = atan2(PathingDirection.y, PathingDirection.x);
	}
	//std::cout << Rot << std::endl;
	return UPDATE_REDRAW;
}


void Cell::SetupUnwalkable()
{
	Vector2d Centroid(0,0);
	double Weight = 0;
	for(int i = 0; i != CellSize; ++i)
	{
		for(int j = 0; j != CellSize; ++j)
		{
			Vector2d LocalPos(i - CellSize * .5, j-CellSize * .5);
			if (!GetWalkable(Pos + LocalPos))
			{
				Weight += 1.0;
				Centroid += LocalPos;
			}
		}
	}
	UnwalkableCenter = Centroid;
	if(Weight != 0)
		UnwalkableCenter /= Weight;
	UnwalkableCenter += Pos;
	UnwalkableWeight = Weight;
}