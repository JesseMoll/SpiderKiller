#pragma once
#include "Vector2d.h"

void SetupWallRepulsionArray();
Vector2d GetWallRepulsion(Vector2d Pos);
bool GetWalkable(Vector2d Pos);
double RadToDeg(double Rad);
double DegToRad(double Deg);
void TurnTo(double & CurrentAngle, double TargetAngle, double TurnSpeed);
double Random(double Max = 1);


