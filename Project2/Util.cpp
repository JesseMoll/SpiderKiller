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

void TurnTo(double & CurrentAngle, double TargetAngle, double TurnSpeed)
{
	double AngleAdder = TurnSpeed;
	double AngletoTurn = (TargetAngle - CurrentAngle);
	while (AngletoTurn < -180)
		AngletoTurn += 360;
	while (AngletoTurn >  180)
		AngletoTurn -= 360;
	if (fabs(AngletoTurn) < TurnSpeed)
		AngleAdder = AngletoTurn;
	else if (AngletoTurn > 0)
		AngleAdder = TurnSpeed;
	else
		AngleAdder = -TurnSpeed;
	CurrentAngle += AngleAdder;
	if (CurrentAngle < 0)
		CurrentAngle += 360;
	if (CurrentAngle >= 360)
		CurrentAngle -= 360;
}