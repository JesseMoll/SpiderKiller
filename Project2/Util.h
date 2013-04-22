#pragma once
#include "Vector2d.h"

bool GetWalkable(Vector2d Pos);
double RadToDeg(double Rad);
double DegToRad(double Deg);
void TurnTo(double & CurrentAngle, double TargetAngle, double TurnSpeed);

