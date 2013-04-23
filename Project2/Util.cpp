#include "Util.h"
#include "Vector2d.h"
#include <cmath>


const int TextureWidth = 512;
const int FilterWidth = 3;
Vector2d WallRespulsionArray[TextureWidth][TextureWidth];
const double FilterSigma = 2.0;


void SetupWallRepulsionArray()
{
	Vector2d FilterArray[FilterWidth * 2 + 1][FilterWidth * 2 + 1];
	double GaussSum = 0;
	for (int x = -FilterWidth;x <= FilterWidth;x++)
	{
		for (int y = -FilterWidth;y <= FilterWidth;y++)
		{
			Vector2d lpos(x, y);
			Vector2d Mult = Vector2d::exp(-(lpos * lpos) / (2*FilterSigma*FilterSigma));
			GaussSum += Mult.x;
			std::cout << Mult << std::endl;
			FilterArray[x + FilterWidth][y + FilterWidth] = Mult;
		}
	}
	//return;
	for (int i = 0;i < TextureWidth;i++)
	{
		for (int j = 0; j < TextureWidth;j++)
		{
			WallRespulsionArray[i][j] = Vector2d(0,0);
			Vector2d gPos = Vector2d(i,j);
			//only need to calculate for valid spots
			if(GetWalkable(gPos))
				for (int x = -FilterWidth;x <= FilterWidth;x++)
				{
					for (int y = -FilterWidth; y <= FilterWidth;y++)
					{
						Vector2d lpos(x, y);
					 
						Vector2d Mult = FilterArray[x + FilterWidth][y + FilterWidth];
						if (!GetWalkable(gPos + lpos))
							Mult = -Mult;
						WallRespulsionArray[i][j] += Mult * lpos;
					}
				}
			WallRespulsionArray[i][j] /= GaussSum;
			
		}
	 }
}

Vector2d GetWallRepulsion(Vector2d Pos)
{
	return WallRespulsionArray[(int)Pos.x][(int)Pos.y];
}

bool GetWalkable(Vector2d Pos)
{
	if(abs(Pos.x - 256) < 8) return true;
	if(abs(Pos.y - 256) < 8) return true;
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

double Random(double Max)
{
	return Max * ((double)rand() / (double)RAND_MAX);
}