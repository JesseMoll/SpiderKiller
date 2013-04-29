#pragma once
#include "drawable.h"
#include "Projectile.h"



class Weapon :
	public Drawable
{
public:

	enum EquipStatus
	{
		EQUIP_NONE,
		EQUIP_LEFT,
		EQUIP_RIGHT,
		EQUIP_SPACE,
	};

	Weapon(Drawable* _Parent, GLuint _Texture = 0, double _FireRate = 1.0, Projectile* _ProjectileToFire = 0);
	~Weapon(void);
	void Equip(EquipStatus NewStatus);
//Overridden Functions
	//Dont draw the weapon
	void draw2(){};
	UpdateResult update2(int ms, GlobalState &GS);
protected:
	double FireRate;
	double FireTimer;
	Projectile* ProjectileToFire;
	EquipStatus Equipped;
};

