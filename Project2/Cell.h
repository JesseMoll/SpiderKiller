#pragma once
#include "Creep.h"
#include <vector>



class Cell;
class Node
{

	public:
		Cell* DstCell;
		Cell* SrcCell;
		double Distance;
		Node(Cell* _SrcCell, Cell* _DstCell, double _Distance):SrcCell(_SrcCell), DstCell(_DstCell),  Distance(_Distance)
		{};
	public:
		static struct NodeComparator
		{
			bool operator() (Node& arg1, Node& arg2)
			{
				return arg1.Distance > arg2.Distance;
			};
		};
};


class Cell :
	public Drawable
{
public:
	Cell(void);
	~Cell(void);
	void AddConnection(Cell* ConnectedCell, double Distance = 1);
	Cell* TargetCell;
	std::vector<Node> Connections;
	std::vector<Creep*> CreepList;
	Vector2d UnwalkableCenter;
	double UnwalkableWeight;
	bool Processed;
	double DistanceFromGoal;
	void SetupUnwalkable();

protected:
	UpdateResult update2(int ms, GlobalState &GS);
};


