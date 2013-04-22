#include "Util.h"
#include "Vector2d.h"

bool GetWalkable(Vector2d Pos)
{
	if(abs(Pos.x - 256) < 10) return true;
	if(abs(Pos.y - 256) < 10) return true;
	if((Pos.x - 256) * (Pos.x - 256) + (Pos.y - 256) * (Pos.y - 256) < 400) return true;
	return false;
}

double RadToDeg(double Rad)
{
	return Rad * (180.0 / 3.1415926535897932384626433832795);
}

double DegToRad(double Deg)
{
	return Deg / (180.0 / 3.1415926535897932384626433832795);
}