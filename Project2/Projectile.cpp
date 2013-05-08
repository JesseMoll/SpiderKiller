#include "Projectile.h"
#include "creep_manager.h"
#include "Creep.h"
#include "Grid.h"

Projectile::Projectile(Drawable* _Parent, GLuint _Texture, double _Scale, double _Speed, double _Damage, double _MaxDistance, Projectile* _ProjectileToFireOnDeath, int _NumToFireOnDeath,  double _SpreadOnDeath, Vector3d _Color):
	Drawable(_Parent, _Texture, Vector2d(0,0), Vector2d(_Scale,_Scale), _Color),
	Speed(_Speed),
	Damage(_Damage),
	MaxDistance(_MaxDistance),
	ProjectileToFireOnDeath(_ProjectileToFireOnDeath),
	NumToFireOnDeath(_NumToFireOnDeath),
	SpreadOnDeath(_SpreadOnDeath),
	DistanceTravelled(0)
{
}

Projectile* Projectile::clone() const
{
	return new Projectile(*this);
}

Projectile::~Projectile(void)
{
}

void Projectile::onDeath(GlobalState &GS)
{
	if(ProjectileToFireOnDeath != 0)
	{
		double NewRotation = Rot;
		NewRotation -= (double)SpreadOnDeath / 2;
		for(int n = 0; n != NumToFireOnDeath; n++)
		{
			Projectile* new_projectile = ProjectileToFireOnDeath->clone();
			new_projectile->setPos(Pos);
			new_projectile->setRot(NewRotation);
			//Randomly slow down the bullets (so they don't look so fake)
			new_projectile->Speed *= (Random(.2) + .8);

			Parent->AddChild (new_projectile);
			//otherwise the first and last projectiles overlap on a radial weapon
			if(SpreadOnDeath > 3)
				NewRotation += SpreadOnDeath / (double)(NumToFireOnDeath);
			else
				NewRotation += SpreadOnDeath / (double)(NumToFireOnDeath - 1);
		}
	}
}

UpdateResult Projectile::update2(int ms, GlobalState &GS)
{
	Vector2d PosAdder(cos(Rot), sin(Rot));
	PosAdder *= Speed * (ms / 1000.0);
	//Will the new position be valid?
	DistanceTravelled += PosAdder.length();
	if (DistanceTravelled >= MaxDistance)
	{
		onDeath(GS);
		return UPDATE_DELETE;
	}
	if(GetWalkable(Pos + PosAdder))
	{
		Pos += PosAdder;
		Rect2d ProjectileRect = GetBoundingRect();
		creep_manager* CM = static_cast<creep_manager*> (GS.TheCreepManager);
		Grid* G = GS.TheGrid;
		auto NearbyCells = G->get_nearby_cells(Pos, CellSize * 2);
		for (auto itr = NearbyCells.begin(); itr != NearbyCells.end(); ++itr){
			for (auto itr2 = (*itr)->CreepList.begin(); itr2 != (*itr)->CreepList.end(); ++itr2){
				if((*itr2)->checkOverlap(Pos, Scale.x))
				{
					//deal damage to the creep based on the projectile's damage
					Damage = ((Creep*)(*itr2))->Damage(Damage);
					if (Damage <= 0)
					{
						onDeath(GS);
						return UPDATE_DELETE;
					}
				}
			}
		}
	}
	else
	{
		onDeath(GS);
		return UPDATE_DELETE;
	}
	return UPDATE_REDRAW;
}