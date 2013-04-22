#include "Hero.h"


Hero::Hero(Vector2d _Pos, Drawable* _Parent, GLuint _Texture):Drawable(_Pos, _Parent, _Texture),AnimationTime(0)
{
	Color = Vector3d(0,1,0);
}


//Turn from an angle to another angle at a set speed
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




UpdateResult Hero::update2(int ms, GlobalState &GS)
{
	
	Vector2d WalkingDirection(0,0);

	//Set walking direction (forward/backward) and angle depending on which key is pressed
	if (GS.KeyDown(W_KEY))
	{
		WalkingDirection.y = 1;
	}
	if (GS.KeyDown(S_KEY))
	{
		WalkingDirection.y = -1;
	}
	if (GS.KeyDown(D_KEY))
	{
		WalkingDirection.x = 1;
	}
	if (GS.KeyDown(A_KEY))
	{
		WalkingDirection.x = -1;
	}

	
	//If we are walking
	if (WalkingDirection.x != 0 || WalkingDirection.y != 0)
	{

		
		double NewAngle = RadToDeg(atan2(WalkingDirection.y,WalkingDirection.x));

		//Man turns at 10 degrees per frame
		TurnTo(Rot, NewAngle, 10.0);
		//Walk in the direction that he is facing
		Vector2d PosAdder;
		PosAdder.x = cos(DegToRad(Rot)) * GS.MoveSpeed;
		PosAdder.y = sin(DegToRad(Rot)) * GS.MoveSpeed;

		if(GetWalkable(Pos + PosAdder))
			Pos += PosAdder;
		
		//Update the walking animation
		AnimationTime += ms / 200.0f;

	}


		GS.CamPos = Pos;


	return UPDATE_REDRAW;
}


Hero::~Hero(void)
{
}
