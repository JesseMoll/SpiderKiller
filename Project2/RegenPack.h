#pragma once
#include "projectile.h"
class RegenPack :
	public Projectile
{
public:
	RegenPack(Drawable* _Parent, double _RegenRate = 50, double _RegenTime = 15000);
	~RegenPack(void);
	void draw2();
	virtual RegenPack* clone() const;
protected:
	double RegenRate;
	double RegenTime;
	double ElapsedTime;
//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);
};
