#include "Util.h"
#include "Vector2d.h"
#include <cmath>
#include "texture_manager.h"



texture Tex;

void SetupTexture()
{
	Tex = texture_manager::get_texture("Level2.bmp");
}
bool GetWalkable(Vector2d Pos)
{
	//If transparent, we can't walk there, its a wall
	//We need a better func than this (built into the level class)
	//I just really like seeing things work
	if(Pos.x >= LevelSize || Pos.y >= LevelSize || Pos.x < 0 || Pos.y < 0)
		return false;
	if(Tex[((int)(Pos.y) * LevelSize + (int)Pos.x) * 4 + 3] != 0)
		return true;
	return false;
}

inline void swap(int &n0, int &n1)
{
	int temp = n0;
	n0 = n1;
	n1 = temp;
}

//using Bresenham's algorithm
//http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
bool CheckVisibility(Vector2d &p1, Vector2d &p2)
{
	int x0 = p1.x;
	int y0 = p1.y;
	int x1 = p2.x;
	int y1 = p2.y;
	bool RetVal = true;
	 bool steep = abs(y1 - y0) > abs(x1 - x0);
     if (steep)
	 {
         swap(x0, y0);
         swap(x1, y1);
	 }
     if (x0 > x1)
	 {
         swap(x0, x1);
         swap(y0, y1);
	 }
     int deltax = x1 - x0;
     int deltay = abs(y1 - y0);
     int error = deltax / 2;
     int ystep;
     int y = y0;
     if (y0 < y1)
		ystep = 1;
	 else 
		ystep = -1;
     for (int x = x0; x <= x1; x++)
	 {
		 bool IsWalkable;
         if (steep) 
			 IsWalkable = GetWalkable(Vector2d(y,x));
		 else
			 IsWalkable = GetWalkable(Vector2d(x,y));
		 if (!IsWalkable)
			 RetVal = false;
         error = error - deltay;
         if (error < 0)
		 {
			y = y + ystep;
			error = error + deltax;
		 }
	 }
	 return RetVal;
}


void TurnTo(double & CurrentAngle, const double TargetAngle, const double TurnSpeed)
{
	/*
	double curX = cos(CurrentAngle);
	double curY = sin(CurrentAngle);
	double tgtX = cos(TargetAngle);
	double tgtY = sin(TargetAngle);
	double nextX = curX * .8 + tgtX * .2;
	double nextY = curY * .8 + tgtY * .2;
	CurrentAngle = atan2(nextY, nextX);
	return;
	*/
	
	double AngleAdder = TurnSpeed;
	double AngletoTurn = (TargetAngle - CurrentAngle);
	//std::cout << TargetAngle << " " << CurrentAngle << std::endl;
	double X = cos(AngletoTurn);
	double Y = sin(AngletoTurn);
	AngletoTurn = atan2(Y, X);
	//while (AngletoTurn < -PI)
	//	AngletoTurn += 2*PI;
	//while (AngletoTurn >  PI)
	//	AngletoTurn -= PI;
	if (fabs(AngletoTurn) < TurnSpeed)
		AngleAdder = AngletoTurn;
	else if (AngletoTurn > 0)
		AngleAdder = TurnSpeed;
	else
		AngleAdder = -TurnSpeed;
	CurrentAngle += AngleAdder;
	X = cos(CurrentAngle);
	Y = sin(CurrentAngle);
	CurrentAngle = atan2(Y, X);
}

double Random(double Max)
{
	return Max * ((double)rand() * (1 / (double)RAND_MAX));
}

Vector2d GetForce(Vector2d &p1, Vector2d &p2)
{
	double Distance = std::max(Vector2d::distance(p1, p2), .1);
	//Return a vector in the direction of p2 which has magnitude 1/r^2
	return (p2 - p1) / (Distance * Distance * Distance);
}