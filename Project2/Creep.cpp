#include "Creep.h"

Creep::Creep(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, double _Scale, double _Rot, double _Speed, double _TurnSpeed, Vector3d _Color):
	Drawable(_Parent, _Texture, _Pos, Vector2d(_Scale,_Scale), _Color),
	Speed(_Speed),
	TurnSpeed(_TurnSpeed)
{
	
	Rot = _Rot;
}

Creep::Creep(Creep * c):
	Drawable(c->Parent, c->Texture, c->Pos, c->Scale, c->Color),
	Speed(c->Speed),
	TurnSpeed(c->TurnSpeed)
{
	Rot = c->Rot;
}

Creep::~Creep(void)
{
}


UpdateResult Creep::update2(int ms, GlobalState &GS)
{
	
	

	Vector2d WalkingDirection = GS.HeroPos - Pos;
	WalkingDirection.normalize();
	Vector2d WallRepulsion = GetWallRepulsion(Pos);
	Vector2d CreepRepulsion (0,0);
	//Loop through all of the creeps (OMG THIS DOESNT WORK WELL... O(n^2) is bad...)
	//Will fix in the future, for now, compiling as Release works for me
	for (std::list<Drawable*>::iterator itr = Parent->begin(); itr != Parent->end(); itr++)
	{
		Vector2d VecToCreep = (*itr)->Pos - Pos;
		const double PreferredDistance = (1.5 + 1.5 * (Scale.x + (*itr)->Scale.x));
		//Creep is within range (twice the preferred distance) and is not itself
		if (VecToCreep.length() < PreferredDistance * 2 && *itr != this)
		{
			double x = VecToCreep.length() / PreferredDistance;
			//This just works, a function which is:
				//infinite at 0 (to prevent collisions)
				//0 at 1 (to maintain a preferred distance)
				//negative greater than 1 (forces creeps to bunch up)
				//0 at infinity (a creep far away will not move toward other creep)
			//Should give a pretty smooth way to maintain distance
			double Attractivity = tanh(1-x)/(x*x);

			//Big creeps are less influenced by smaller ones
			Attractivity *= (*itr)->Scale.x / Scale.x;
			Attractivity *= (*itr)->Scale.x / Scale.x;
			CreepRepulsion -=  Vector2d::normalize(VecToCreep) * Attractivity;
		}
	}

	WalkingDirection += WallRepulsion * 10;
	WalkingDirection += CreepRepulsion;
	double NewAngle = RadToDeg(atan2(WalkingDirection.y,WalkingDirection.x));

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed * (ms / 1000.0));
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(DegToRad(Rot)), sin(DegToRad(Rot)));
	PosAdder *= Speed * (ms / 1000.0);
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