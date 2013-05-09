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

	Weapon(Drawable* _Parent, GLuint _Texture = 0, double _FireRate = 1.0, Projectile* _ProjectileToFire = 0, EquipStatus EquipSlot = EQUIP_LEFT);
	~Weapon(void);
	void Equip();
	void Unequip();
	EquipStatus getEquipSlot(){return EquipSlot;};
	double getCoolDown(){return FireTimer / FireRate;};
//Overridden Functions
	//Dont draw the weapon
	void draw2(){};
	UpdateResult update2(int ms, GlobalState &GS);
protected:
	double FireRate;
	double FireTimer;
	Projectile* ProjectileToFire;
	bool Equipped;
	EquipStatus EquipSlot;
};

