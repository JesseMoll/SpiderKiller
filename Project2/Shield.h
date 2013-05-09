#pragma once
#include "projectile.h"
class Shield :
	public Projectile
{
public:
	Shield(Drawable* _Parent, double _Health = 500, double _DecayRate = 10);
	~Shield(void);
	void draw2();
	virtual Shield* clone() const;
protected:
	double DecayRate;

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);
};

