#include "Util.h"
#include "Vector2d.h"
#include <cmath>
#include "texture_manager.h"


const int TextureWidth = 512;
const int FilterWidth = 4;
Vector2d WallRespulsionArray[TextureWidth][TextureWidth];
const double FilterSigma = 1.5;
texture Tex;

void SetupTexture()
{
	Tex = texture_manager::get_texture("Level1.bmp");
}
bool GetWalkable(Vector2d Pos)
{
	//If transparent, we can't walk there, its a wall
	//We need a better func than this (built into the level class)
	//I just really like seeing things work
	if(Pos.x >= 512 || Pos.y >= 512 || Pos.x < 0 || Pos.y < 0)
		return false;
	if(Tex[((int)(Pos.y) * 512 + (int)Pos.x) * 4 + 3] != 0)
		return true;
	return false;
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