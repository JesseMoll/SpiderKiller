#include "Hero.h"


Hero::Hero(Drawable* _Parent, GLuint _Texture, Vector2d _Pos):Drawable(_Parent, _Texture, _Pos),
	AnimationTime(0)
{
	Color = Vector3d(0,1,0);
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
		TurnTo(Rot, NewAngle, 720.0 * (ms / 1000.0));
		//Walk in the direction that he is facing
		Vector2d PosAdder(cos(DegToRad(Rot)), sin(DegToRad(Rot)));
		PosAdder *= (ms / 1000.0) * GS.MoveSpeed;
		//Will the new position be valid?
		if(GetWalkable(Pos + PosAdder))
			Pos += PosAdder;
		
		//Update the walking animation
		AnimationTime += ms / 200.0f;

	}


		GS.HeroPos = Pos;


	return UPDATE_REDRAW;
}


Hero::~Hero(void)
{
}
