#include "Grid.h"
#include "Creep.h"
#include <queue>


Grid::Grid(void)
{
	const double SQRT2 = sqrt(2.0);
	const double SQRT5 = sqrt(5.0);
	for(int i = 0; i != GridSize; ++i)
	{
		for(int j = 0; j != GridSize; ++j)
		{
			cells[i][j].CreepList.reserve(8);
			cells[i][j].setPos(Vector2d(i + .5,j + .5)*CellSize);
			cells[i][j].SetupUnwalkable();
			if(cells[i][j].UnwalkableWeight != MaxWeight)
			{
				AddChild(&cells[i][j]);
				cells[i][j].AddConnection(GetCell(i-1,j), 1.0);
				cells[i][j].AddConnection(GetCell(i,j-1), 1.0);
				cells[i][j].AddConnection(GetCell(i+1,j), 1.0);
				cells[i][j].AddConnection(GetCell(i,j+1), 1.0);

				cells[i][j].AddConnection(GetCell(i-1,j-1), SQRT2);
				cells[i][j].AddConnection(GetCell(i+1,j-1), SQRT2);
				cells[i][j].AddConnection(GetCell(i+1,j+1), SQRT2);
				cells[i][j].AddConnection(GetCell(i-1,j+1), SQRT2);

				cells[i][j].AddConnection(GetCell(i-2,j-1), SQRT5);
				cells[i][j].AddConnection(GetCell(i+2,j-1), SQRT5);
				cells[i][j].AddConnection(GetCell(i+2,j+1), SQRT5);
				cells[i][j].AddConnection(GetCell(i-2,j+1), SQRT5);

				cells[i][j].AddConnection(GetCell(i-1,j-2), SQRT5);
				cells[i][j].AddConnection(GetCell(i+1,j-2), SQRT5);
				cells[i][j].AddConnection(GetCell(i+1,j+2), SQRT5);
				cells[i][j].AddConnection(GetCell(i-1,j+2), SQRT5);
			}
		}
	}
}

void Grid::UpdatePathing(Cell* Target)
{
	for(int i = 0; i != GridSize; ++i)
		for(int j = 0; j != GridSize; ++j)
		{
			cells[i][j].Processed = false;
			cells[i][j].TargetCell = NULL;
		}

	std::priority_queue<Node, std::vector<Node>, Node::NodeComparator> OpenList;

	Node RootNode = Node(0, Target, 0);
	OpenList.push(RootNode);
	while(!OpenList.empty())
	{
		Node NodeToProcess = OpenList.top();
		OpenList.pop();
		if(!NodeToProcess.DstCell->Processed)
		{
			NodeToProcess.DstCell->Processed = true;
			NodeToProcess.DstCell->TargetCell = NodeToProcess.SrcCell;
			for(auto itr = NodeToProcess.DstCell->Connections.begin(); itr != NodeToProcess.DstCell->Connections.end(); ++itr)
			{
				Node NewNode = *itr;
				NewNode.Distance += NodeToProcess.Distance;
				OpenList.push(NewNode);
			}
		}
	}
}

Cell* Grid::GetCell(int x, int y)
{
	if (x < 0 || y < 0 || x >= GridSize || y >= GridSize)
		return NULL;
	return &cells[x][y];
}

Cell* Grid::GetCell(Vector2d &Pos)
{
	return GetCell(static_cast<int>(Pos.x / CellSize), static_cast<int>(Pos.y / CellSize));
}

void Grid::draw2()
{
}

std::list<Cell*> Grid::get_nearby_cells(Vector2d creep_pos, double radius)
{
	std::list<Cell*> RetVal;
	int CheckWidth = static_cast<int>(radius / CellSize);
	int x = static_cast<int>(creep_pos.x / CellSize);
	int y = static_cast<int>(creep_pos.y / CellSize);
	int x_min = std::max(0,x - CheckWidth);
	int x_max = std::min(GridSize - 1,x + CheckWidth);
	int y_min = std::max(0,y - CheckWidth);
	int y_max = std::min(GridSize - 1,y + CheckWidth);

	for(int i = x_min; i <= x_max; ++i)
	{
		for(int j = y_min; j <= y_max; ++j)
		{
				//if(Vector2d::distance_squared(creep_pos, c->getLocalPos()) < radius_squared)
				RetVal.push_back(&cells[i][j]);
		}
	}
	return RetVal;
}

void Grid::add_creep(Drawable* c)
{
	Vector2d CreepPos = c->getPos();
	GetCell(CreepPos)->CreepList.push_back(static_cast<Creep*>(c));
}

void Grid::UpdateGrid(std::list<Drawable*>* ChildList)
{
	for(int i = 0; i != GridSize; ++i)
	{
		for(int j = 0; j != GridSize; ++j)
		{
			cells[i][j].CreepList.clear();
		}
	}
	for(auto itr = ChildList->begin(); itr != ChildList->end(); ++itr)
	{
		add_creep(static_cast<Creep*>(*itr));
	}
}

UpdateResult Grid::update2(int ms, GlobalState &GS)
{
	static Cell* LastTarget = NULL;
	Cell* TargetCell = GetCell(GS.HeroPos);
	if(TargetCell != LastTarget)
	{
		LastTarget = TargetCell;
		UpdatePathing(TargetCell);
	}
	


	return UPDATE_NONE;
}

Grid::~Grid(void)
{
}



