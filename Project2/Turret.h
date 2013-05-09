#pragma once
#include "projectile.h"
class Turret :
	public Projectile
{
public:
	Turret(Drawable* _Parent, Projectile* _ProjectileToFire, double _FireRate, unsigned int _Ammo = 100);
	~Turret(void);
	void draw2();
	virtual Turret* clone() const;
protected:
	double FireRate;
	double FireTimer;
	unsigned int Ammo;
//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);
};
