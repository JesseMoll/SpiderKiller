#include "creep_manager.h"
#include "Creep.h"
#include <algorithm>

class creep_manager;
Creep::Creep(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, double _Health, double _Scale, double _Rot, double _Speed, double _TurnSpeed, Vector3d _Color):
	Drawable(_Parent, _Texture, _Pos, Vector2d(_Scale,_Scale), _Color),
	Health(_Health),
	Speed(_Speed),
	TurnSpeed(_TurnSpeed)
{
	Rot = _Rot;
	
}

Creep::Creep(Creep * c, Vector2d _Pos, double _Rot):
	Drawable(c->Parent, c->Texture, _Pos, c->Scale, c->Color),
	Speed(c->Speed),
	TurnSpeed(c->TurnSpeed),
	Health(c->Health)
{
	if(_Pos.x == 0 && _Pos.y == 0)
		Pos = c->Pos;
	if(_Rot > 10000)
		Rot = c->Rot;
	Rot = _Rot;
}

Creep::~Creep(void)
{
}


UpdateResult Creep::update2(int ms, GlobalState &GS)
{
	//It's dead
	if(Health == 0)
		return UPDATE_DELETE;

	

	Vector2d WallRepulsion = GetWallRepulsion(Pos);
	Vector2d CreepRepulsion (0,0);
	//Loop through all of the creeps (OMG THIS DOESNT WORK WELL... O(n^2) is bad...)
	//Will fix in the future, for now, compiling as Release works for me
	creep_manager* CM = static_cast<creep_manager*> (Parent);
	auto NearbyCreep = CM->get_nearby_creep(Pos, 6);
	for (auto itr = NearbyCreep.begin(); itr != NearbyCreep.end(); ++itr)
	{
		Vector2d VecToCreep = (*itr)->Pos - Pos;
		const double PreferredDistance = 1 + (Scale.x + (*itr)->Scale.x) * 2;
		double CurrentDistance = VecToCreep.length();
		double x = std::max(CurrentDistance, .1) / (PreferredDistance);
		//This just works, a function which is:
			//infinite at 0 (to prevent collisions)
			//0 at 1 (to maintain a preferred distance)
			//negative greater than 1 (forces creeps to bunch up)
			//0 at infinity (a creep far away will not move toward other creep)
		//Should give a pretty smooth way to maintain distance
		double Attractivity = tanh(1-x)/(x*x);

		//Big creeps are less influenced by smaller ones
		double SizeRatio = (*itr)->Scale.x / Scale.x;
		Attractivity *= SizeRatio * SizeRatio;
		//Only affected by equal or bigger creep
		//if ((*itr)->Scale.x >= Scale.x)
		CreepRepulsion -=  VecToCreep * Attractivity;
	}

	Vector2d WalkingDirection = Vector2d::normalize(GS.HeroPos - Pos) * 10;
	WalkingDirection += WallRepulsion * 10;
	WalkingDirection += CreepRepulsion;
	double NewAngle = atan2(WalkingDirection.y,WalkingDirection.x);

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed * (ms / 1000.0));
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(Rot), sin(Rot));
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

double Creep::Damage(double dmg)
{
	//Returns the overkill damage (projectiles will pass through if they have more damage)
	double RetVal = 0;
	Health -= dmg;
	if(Health < 0)
	{
		RetVal = -Health;
		Health = 0;
	}
	return RetVal;
}