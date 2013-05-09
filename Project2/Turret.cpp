#include "Turret.h"
#include "texture_manager.h"
#include "Grid.h"

Turret::Turret(Drawable* _Parent, Projectile* _ProjectileToFire, double _FireRate, unsigned int _Ammo):
	Projectile(_Parent,texture_manager::get_texture_name("Turret.bmp"), 8, 0, 0,100, _ProjectileToFire),
	FireRate(_FireRate),
	FireTimer(_FireRate),
	Ammo(_Ammo)
{
}

Turret* Turret::clone() const
{
	return new Turret(*this);
}

Turret::~Turret(void)
{
}

void Turret::draw2()
{
	Drawable::draw2();
}

UpdateResult Turret::update2(int ms, GlobalState &GS)
{
	bool Firing = false;
	Vector2d ShootingDirection = Vector2d(0,0);
	Grid* G = GS.TheGrid;
	std::list<Cell*> NearbyCells;
	G->get_nearby_cells(NearbyCells, Pos, CellSize * 12);
	for (auto itr = NearbyCells.begin(); itr != NearbyCells.end(); ++itr)
	{
		for (auto itr2 = (*itr)->CreepList.begin(); itr2 != (*itr)->CreepList.end(); ++itr2)
		{
			if(CheckVisibility(Pos, (*itr)->getPos()))
				ShootingDirection += GetForce(Pos, (*itr)->getPos());
		}
	}
	if(ShootingDirection.x != 0 && ShootingDirection.y != 0)
	{
		Firing = true;
		TurnTo(Rot, atan2(ShootingDirection.y, ShootingDirection.x), 8 * (ms / 1000.0));
	}
	FireTimer -= ms;
	if(FireTimer < 0)
	{
		if(Firing)
		{

			FireTimer += FireRate;
			Projectile* new_projectile = ProjectileToFireOnDeath->clone();
			new_projectile->setPos(Pos);
			new_projectile->setRot(Rot);
			Parent->AddChild (new_projectile);
			//Out of ammo
			if(--Ammo == 0)
				return UPDATE_DELETE;
		}
		else
		{
			FireTimer = 0;
		}
	}

	return UPDATE_REDRAW;
}
