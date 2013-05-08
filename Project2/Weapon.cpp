#include "Weapon.h"


Weapon::Weapon(Drawable* _Parent, GLuint _Texture, double _FireRate, Projectile* _ProjectileToFire, EquipStatus _EquipSlot):
	Drawable(_Parent, _Texture),
	FireRate(_FireRate),
	FireTimer(0),
	ProjectileToFire(_ProjectileToFire),
	Equipped(false),
	EquipSlot(_EquipSlot)
{
}


Weapon::~Weapon(void)
{
}

void Weapon::Equip()
{
	Equipped = true;
}

void Weapon::Unequip()
{
	Equipped = false;
}

UpdateResult Weapon::update2(int ms, GlobalState &GS)
{
	//Firing
	if(Equipped)
	{
		FireTimer -= ms;
		if(FireTimer < 0)
		{
			bool Firing = false;
			if (GS.KeyDown(LEFT_MOUSE) && EquipSlot == EQUIP_LEFT)
				Firing = true;
			if (GS.KeyDown(RIGHT_MOUSE) && EquipSlot == EQUIP_RIGHT)
				Firing = true;
			if (GS.KeyDown(SPACE_KEY) && EquipSlot == EQUIP_SPACE && GS.HeroFocus >= 25)
			{
				GS.HeroFocus -= 25;
				Firing = true;
			}
			if (Firing)
			{
				FireTimer += FireRate;
				Vector2d DirectionVector = GS.MousePos - GS.WindowSize * .5;
				double ProjectileRot = atan2(-DirectionVector.y, DirectionVector.x);
				Projectile* new_projectile = ProjectileToFire->clone();
				new_projectile->setPos(GS.HeroPos);
				new_projectile->setRot(ProjectileRot);
					
				Parent->AddChild(new_projectile);
			}
			else
			{
				FireTimer = 0;
			}
		}
	}
	return UPDATE_NONE;
}