#include "Grid.h"
#include "Creep.h"
#include <queue>


Grid::Grid(void):debug(false)
{

	cells.resize(GridSize);
	for(int i = 0; i != GridSize; ++i)
	{
		cells[i].resize(GridSize);
		for(int j = 0; j != GridSize; ++j)
		{
			cells[i][j].CreepList.reserve(8);
			cells[i][j].setPos(Vector2d(i + .5,j + .5)*CellSize);
			cells[i][j].SetupUnwalkable();
		}
	}

	for(int i = 0; i != GridSize; ++i)
	{
		for(int j = 0; j != GridSize; ++j)
		{	
			if(cells[i][j].UnwalkableWeight != MaxWeight)
			{
				Addconnections(i, j, 0, 1);
				Addconnections(i, j, 1, 0);
				Addconnections(i, j, 1, 1);
				Addconnections(i, j, 2, 1);
				Addconnections(i, j, 1, 2);
				if(cells[i][j].Connections.size()!= 0)
					AddChild(&cells[i][j]);
			}
		}
	}
}

void Grid::Addconnections(const int i, const int j, const int iAdder, const int jAdder)
{
	//Centeri = i + 
	double Dist = sqrt(iAdder * iAdder + jAdder * jAdder);
	
	Cell* c1 = GetCell(i,j);
	Cell* c2 = GetCell(i+iAdder,j+jAdder);
	Cell* c3 = GetCell(i-iAdder,j+jAdder);
	Cell* c4 = GetCell(i+iAdder,j-jAdder);
	Cell* c5 = GetCell(i-iAdder,j-jAdder);

	if(c2 && CheckVisibility(c1->Pos,c2->Pos))
		cells[i][j].AddConnection(GetCell(i+iAdder,j+jAdder), Dist);
	if(iAdder != 0)
		if(c3 && CheckVisibility(c1->Pos,c3->Pos))
			cells[i][j].AddConnection(GetCell(i-iAdder,j+jAdder), Dist);
	if(jAdder != 0)
		if(c4 && CheckVisibility(c1->Pos,c4->Pos))
			cells[i][j].AddConnection(GetCell(i+iAdder,j-jAdder), Dist);
	if(iAdder != 0 && jAdder != 0)
		if(c5 && CheckVisibility(c1->Pos,c5->Pos))
			cells[i][j].AddConnection(GetCell(i-iAdder,j-jAdder), Dist);
	

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

void Grid::draw()
{
	if(debug)
		for (auto dPtr = Children.begin();dPtr != Children.end(); ++dPtr) {
			(*dPtr)->draw();
		}
}

void Grid::get_nearby_cells(std::list<Cell*> &CellList, const Vector2d &creep_pos, const double &radius)
{
	int CheckWidth = static_cast<int>(radius / CellSize);
	int x = static_cast<int>(creep_pos.x / CellSize);
	int y = static_cast<int>(creep_pos.y / CellSize);
	int x_min = std::max(0,x - CheckWidth);
	int x_max = std::min(GridSize - 1,x + CheckWidth);
	int y_min = std::max(0,y - CheckWidth);
	int y_max = std::min(GridSize - 1,y + CheckWidth);
	double radius_squared = radius * radius;
	for(int i = x_min; i <= x_max; ++i)
	{
		for(int j = y_min; j <= y_max; ++j)
		{
			if(Vector2d::distance_squared(creep_pos, cells[i][j].Pos) < radius_squared)
				CellList.push_back(&cells[i][j]);
		}
	}
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
	debug = GS.Debug;
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



