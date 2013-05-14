#include "creep_manager.h"
#include "Creep.h"
#include <algorithm>

class creep_manager;
Creep::Creep(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, double _Health, double _Scale, double _Rot, double _Speed, double _TurnSpeed, Vector4d _Color, Creep* _SpawnOnDeath, Creep* _SpawnPeriodic, double _PeriodicSpawnRate, double _PeriodicSpawnNum, double _OnDeathSpawnNum):
	Drawable(_Parent, _Texture, _Pos, Vector2d(_Scale,_Scale), _Color),
	Health(_Health),
	Speed(_Speed),
	TurnSpeed(_TurnSpeed),
	SpawnOnDeath(_SpawnOnDeath),
	SpawnPeriodic(_SpawnPeriodic),
	PeriodicSpawnRate(_PeriodicSpawnRate),
	PeriodicSpawnTimer(_PeriodicSpawnRate),
	PeriodicSpawnNum(_PeriodicSpawnNum),
	OnDeathSpawnNum(_OnDeathSpawnNum)
{
	Rot = _Rot;
	
}

Creep* Creep::clone() const
{
	return new Creep(*this);
}

Creep::~Creep(void)
{

}


UpdateResult Creep::update2(int ms, GlobalState &GS)
{
	//It's dead
	if(Health == 0)
	{

		if(Parent->Children.size() < MaxCreep && SpawnOnDeath != NULL)
		{
			for(int n = 0; n != OnDeathSpawnNum; n++)
			{
				Creep* NewCreep = SpawnOnDeath->clone();
				NewCreep->setPos(Pos + Vector2d(Random(10.0) - 5.0, Random(10.0) - 5.0)); 
				NewCreep->setRot(Rot + Random(.4) - .2);
				GS.TheCreepManager->AddChild(NewCreep);
			}
		}

		//Gain focus based on how big of a bug we just killed;
		GS.HeroFocus = std::min(GS.HeroFocus + Scale.x * Scale.x * Scale.x * FocusGainMult, GS.HeroMaxFocus);
		return UPDATE_DELETE;
	}
	
	PeriodicSpawnTimer -= ms;
	if(PeriodicSpawnTimer < 0 && Parent->Children.size() < MaxCreep && SpawnPeriodic != NULL)
	{
		PeriodicSpawnTimer = PeriodicSpawnRate;
		for(int n = 0; n != PeriodicSpawnNum; n++)
		{
			Creep* NewCreep = SpawnPeriodic->clone();
			NewCreep->setPos(Pos + Vector2d(Random(10.0) - 5.0, Random(10.0) - 5.0)); 
			NewCreep->setRot(Rot + Random(.4) - .2);
			GS.TheCreepManager->AddChild(NewCreep);
		}
	}

	Vector2d WallRepulsion(0,0);
	Vector2d CreepRepulsion (0,0);
	Vector2d NearbyCreepVelocity (0,0);
	Vector2d PathingVector(0,0);

	std::list<Cell*> NearbyCells;
	GS.TheGrid->get_nearby_cells(NearbyCells, Pos, CellSize * 2);
	int NumNearby = 0;
	for (auto itr = NearbyCells.begin(); itr != NearbyCells.end(); ++itr)
	{
		
		Vector2d VecToCell = ((*itr)->Pos - Pos);
		double LengthToCellSquared = std::max(VecToCell.length_squared(), .1);
		PathingVector += Vector2d(cos((*itr)->getRot()), sin((*itr)->getRot()))  / (LengthToCellSquared);

		WallRepulsion += GetForce((*itr)->UnwalkableCenter, Pos) * (*itr)->UnwalkableWeight;

		Cell* CurrentCell = *itr;

		for (auto itr2 = CurrentCell->CreepList.begin(); itr2 != CurrentCell->CreepList.end(); ++itr2)
		{	
			Creep* CurrentCreep = *itr2;
			if(CurrentCreep == NULL)
				break;

			Vector2d VecToCreep = CurrentCreep->Pos - Pos;
		
			double CombinedRadius = (Scale.x + CurrentCreep->Scale.x) * 1.2;
			//Use distance squared so we don't need to calc the sqrt
			double CurrentDistanceSquared = std::max(VecToCreep.length_squared(), .1);
			const double PreferredDistanceSquared = (CombinedRadius + .5) * (CombinedRadius + .5);
			
			if(CurrentCreep != this && CurrentDistanceSquared < PreferredDistanceSquared * 2.5)
			{
				NumNearby++;
				Vector2d CreepVelocity (cos(CurrentCreep->Rot),sin(CurrentCreep->Rot));
				NearbyCreepVelocity +=  CreepVelocity / (CurrentDistanceSquared);

				double x = CurrentDistanceSquared / (PreferredDistanceSquared);


				//This just works, a function which is:
					//infinite at 0 (to prevent collisions)
					//0 at 1 (to maintain a preferred distance)
					//negative greater than 1 (forces creeps to bunch up)
					//0 at infinity (a creep far away will not move toward other creep)
				//Should give a pretty smooth way to maintain distance
				//http://www.wolframalpha.com/input/?i=tanh%281-x%29%2F%28e%5E%28x*x%29%29+from+0+to+2.5
				double Attractivity = tanh(1-x)*(exp(-x*x));

				double SizeRatio = CurrentCreep->Scale.x / Scale.x;
				Attractivity *= SizeRatio;
				//Attractivity *= SizeRatio;

				CreepRepulsion -=  Vector2d::normalize(VecToCreep) * Attractivity;
			}
		}
	}

	PathingVector.normalize();
	

	Vector2d WalkingDirection = Vector2d(cos(Rot), sin(Rot)); //Our current velocity
	WalkingDirection += PathingVector * PathingForceMult;
	WalkingDirection += GetForce(Pos, GS.HeroPos) * HeroForceMult;
	WalkingDirection += WallRepulsion * WallForceMult;
	
	if(NumNearby != 0)
	{
		WalkingDirection += CreepRepulsion * CreepDistanceForceMult;
		NearbyCreepVelocity.normalize();
		WalkingDirection += NearbyCreepVelocity * CreepVelocityForceMult;
	}

	double NewAngle = atan2(WalkingDirection.y,WalkingDirection.x);

	//Turn toward the hero at the turn speed
	TurnTo(Rot, NewAngle, TurnSpeed * (ms / 1000.0));
	//Walk in the direction that he is facing
	Vector2d PosAdder(cos(Rot), sin(Rot));
	PosAdder *= Speed * (ms / 1000.0);
	//slow down if we are going the wrong way
	PosAdder *= .4 * (cos(Rot - NewAngle) + 1.5);
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