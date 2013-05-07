#pragma once
#include "Vector2d.h"

const double PI = 3.1415926535897932384626433832795;

extern const int LevelSize;
extern const int CellSize;
extern const int MaxWeight;
extern const int GridSize;
extern const double MinView;
extern const double MaxView;
extern const double CameraMoveSpeed;
extern const int MaxCreep;
extern const unsigned int UPDATE_INTERVAL;

//Flocking force multipliers
extern const double WallForceMult;
extern const double PathingForceMult;
extern const double HeroForceMult;
extern const double CreepDistanceForceMult;
extern const double CreepVelocityForceMult;

void SetupTexture();
bool GetWalkable(Vector2d Pos);
bool CheckVisibility(Vector2d &p1, Vector2d &p2);

inline double RadToDeg(double Rad)
{
	return Rad * (180.0 / PI);
}

inline double DegToRad(double Deg)
{
	return Deg * (PI / 180.0);
}

Vector2d GetForce(Vector2d &p1, Vector2d &p2);

void TurnTo(double &CurrentAngle, const double TargetAngle, const double TurnSpeed);
double Random(double Max = 1);


