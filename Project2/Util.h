#pragma once
#include "Vector2d.h"

const double PI = 3.1415926535897932384626433832795;

void SetupWallRepulsionArray();
Vector2d GetWallRepulsion(Vector2d Pos);
bool GetWalkable(Vector2d Pos);

inline double RadToDeg(double Rad)
{
	return Rad * (180.0 / PI);
}

inline double DegToRad(double Deg)
{
	return Deg * (PI / 180.0);
}

void TurnTo(double &CurrentAngle, const double TargetAngle, const double TurnSpeed);
double Random(double Max = 1);


