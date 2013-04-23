#include "Creep.h"

Creep::Creep(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, double _Scale, double _Speed, double _TurnSpeed, Vector3d _Color):
	Drawable(_Parent, _Texture, _Pos, Vector2d(_Scale,_Scale), _Color),
	Speed(_Speed),
	TurnSpeed(_TurnSpeed)
{

}

Creep::Creep(Creep * c):
	Drawable(c->Parent, c->Texture, c->Pos, c->Scale, c->Color),
	Speed(c->Speed),
	TurnSpeed(c->TurnSpeed)
{
}

Creep::~Creep(void)
{
}


UpdateResult Creep::update2(int ms, GlobalState &GS)
{
	
	Vector2d WalkingDirection = GS.HeroPos - Pos;
	WalkingDirection.normalize();
	Vector2d WallRepulsion = GetWallRepulsion(Pos);
	
	/*
	const int GRID_CHECK_WIDTH = 5;
	int n = 0;
	for(int x = -GRID_CHECK_WIDTH; x <= GRID_CHECK_WIDTH; x++)
		for(int y = -GRID_CHECK_WIDTH; y <= GRID_CHECK_WIDTH; y++)
		{
			Vector2d CheckPos(x,y);
			if((x !=0 || y !=0) && GetWalkable(Pos + CheckPos))
			{
				double length = CheckPos.length();
				WallRepulsion += CheckPos / (length * length);
			}
		}
		*/
	//std::cout << n << std::endl;
	WalkingDirection += WallRepulsion;
	double NewAngle = RadToDeg(atan2(WalkingDirection.y,WalkingDirection.x));

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed);
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(DegToRad(Rot)) * Speed, sin(DegToRad(Rot)) * Speed);
	//Will the new position be valid?
	if(GetWalkable(Pos + PosAdder))
		Pos += PosAdder;
	
	//Check if the creep has reached the hero
	Rect2d CreepRect = GetBoundingRect();
	Rect2d HeroRect = GS.TheHero->GetBoundingRect();
	if(CreepRect.overlaps(HeroRect)) 
	{
		//TODO Damage the hero
		return UPDATE_DELETE;
	}
	return UPDATE_REDRAW;
}