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
	double NewAngle = RadToDeg(atan2(WalkingDirection.y,WalkingDirection.x));

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed);
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(DegToRad(Rot)) * Speed, sin(DegToRad(Rot)) * Speed);
	//Will the new position be valid?
	if(GetWalkable(Pos + PosAdder))
		Pos += PosAdder;
	
	return UPDATE_REDRAW;
}