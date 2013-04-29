#include "Weapon.h"


Weapon::Weapon(Drawable* _Parent, GLuint _Texture, double _FireRate, Projectile* _ProjectileToFire):
	Drawable(_Parent, _Texture),
	FireRate(_FireRate),
	FireTimer(0),
	ProjectileToFire(_ProjectileToFire),
	Equipped(EQUIP_NONE)
{
}


Weapon::~Weapon(void)
{
}

void Weapon::Equip(EquipStatus NewStatus)
{
	Equipped = NewStatus;
}

UpdateResult Weapon::update2(int ms, GlobalState &GS)
{
	//Firing
	if(Equipped != EQUIP_NONE)
	{
		FireTimer -= ms;
		if(FireTimer < 0)
		{
			if (GS.KeyDown(LEFT_MOUSE))
			{
				FireTimer += FireRate;
				Vector2d DirectionVector = GS.MousePos - GS.WindowSize * .5;
				double ProjectileRot = atan2(-DirectionVector.y, DirectionVector.x);
				Parent->AddChild(new Projectile(ProjectileToFire, GS.HeroPos,ProjectileRot));
			}
			else
			{
				FireTimer = 0;
			}
		}
	}
	return UPDATE_NONE;
}