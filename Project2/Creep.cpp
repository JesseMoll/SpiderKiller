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
	{
		GS.HeroFocus = std::min(GS.HeroFocus + .1, GS.HeroMaxFocus);
		return UPDATE_DELETE;
	}
	

	Vector2d WallRepulsion(0,0);
	Vector2d CreepRepulsion (0,0);
	Vector2d NearbyCreepVelocity (0,0);
	Vector2d PathingVector(0,0);

	auto NearbyCells = GS.TheGrid->get_nearby_cells(Pos, 8);
	int NumNearby = 0;
	for (auto itr = NearbyCells.begin(); itr != NearbyCells.end(); ++itr)
	{
		
		Vector2d VecToCell = ((*itr)->UnwalkableCenter - Pos);
		double LengthToCell = std::max(VecToCell.length(), .1);
		PathingVector += Vector2d(cos((*itr)->getRot()), sin((*itr)->getRot()))  / (LengthToCell * LengthToCell);
		WallRepulsion -= VecToCell * (*itr)->UnwalkableWeight / (LengthToCell * LengthToCell * LengthToCell);
		//std::cout << WallRepulsion << std::endl;
		Cell* CurrentCell = *itr;

		for (auto itr2 = CurrentCell->CreepList.begin(); itr2 != CurrentCell->CreepList.end(); ++itr2)
		{	
			Creep* CurrentCreep = *itr2;
			if(CurrentCreep == NULL)
				break;

			Vector2d VecToCreep = CurrentCreep->Pos - Pos;
		
			double CurrentDistance = std::max(VecToCreep.length(), .1);
			const double PreferredDistance = (Scale.x + CurrentCreep->Scale.x) * 2;
			
			if(CurrentCreep != this && CurrentDistance < PreferredDistance * 1.5)
			{
				NumNearby++;
				Vector2d CreepVelocity (cos(CurrentCreep->Rot),sin(CurrentCreep->Rot));
				NearbyCreepVelocity +=  CreepVelocity;

				double x = CurrentDistance / (PreferredDistance);
				double Attractivity = tanh(1-x)/(x*x);
				double SizeRatio = CurrentCreep->Scale.x / Scale.x;
				Attractivity *= SizeRatio;

				CreepRepulsion -=  (VecToCreep / CurrentDistance) * Attractivity;
			}
		}
		


		

		//This just works, a function which is:
			//infinite at 0 (to prevent collisions)
			//0 at 1 (to maintain a preferred distance)
			//negative greater than 1 (forces creeps to bunch up)
			//0 at infinity (a creep far away will not move toward other creep)
		//Should give a pretty smooth way to maintain distance
		//double Attractivity = tanh(1-x)/(x*x);

		//Big creeps are less influenced by smaller ones
		//double SizeRatio = (*itr)->Scale.x / Scale.x;
		//Attractivity *= SizeRatio * SizeRatio;
		//Only affected by equal or bigger creep
		//if ((*itr)->Scale.x >= Scale.x)
		//CreepRepulsion -=  VecToCreep * Attractivity;
	}

	Vector2d WalkingDirection = Vector2d::normalize(PathingVector);//Vector2d::normalize(GS.HeroPos - Pos) * .5;
	WalkingDirection += WallRepulsion;
	
	if(NumNearby != 0)
	{
		WalkingDirection += CreepRepulsion;
		WalkingDirection += NearbyCreepVelocity / NumNearby * .5;
	}
	double NewAngle = atan2(WalkingDirection.y,WalkingDirection.x);

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed * (ms / 1000.0));
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(Rot), sin(Rot));
	PosAdder *= Speed * (ms / 1000.0);
	PosAdder *= .4 * (cos(Rot - NewAngle) + 1);
	//Will the new position be valid?
	if(GetWalkable(Pos + PosAdder))
		Pos += PosAdder;
	
	//Check if the creep has reached the hero
	Rect2d CreepRect = GetBoundingRect();
	Rect2d HeroRect = GS.TheHero->GetBoundingRect();
	if(CreepRect.overlaps(HeroRect)) 
	{
		GS.HeroHealth = std::max(0.0, GS.HeroHealth - Health);
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