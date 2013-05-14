#include "CreepProjectile.h"
#include "texture_manager.h"

CreepProjectile::CreepProjectile(Drawable* _Parent, double _Damage, double _Scale, double _Speed, Vector4d _Color):
	Creep(_Parent, texture_manager::get_texture_name("Spider.bmp"), Vector2d(0,0), _Damage, _Scale, 0, _Speed, 0, _Color)
{
}


CreepProjectile::~CreepProjectile(void)
{
}

CreepProjectile* CreepProjectile::clone() const
{
	return new CreepProjectile(*this);
}

UpdateResult CreepProjectile::update2(int ms, GlobalState &GS)
{
	if(Health == 0)
	{
		//Projectile must have been destroyed by Hero's projectiles
		return UPDATE_DELETE;
	}
	

	Vector2d PosAdder(cos(Rot), sin(Rot));
	PosAdder *= Speed * (ms / 1000.0);

	//Will the new position be valid?
	if(GetWalkable(Pos + PosAdder))
		Pos += PosAdder;
	else
		return UPDATE_DELETE;
	
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