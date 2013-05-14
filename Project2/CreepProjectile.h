#pragma once
#include "creep.h"
class CreepProjectile :
	public Creep
{
public:
	CreepProjectile(Drawable* _Parent, double _Damage = 5, double _Scale = .75, double _Speed = 100, Vector4d _Color = Vector4d(1,0,0,1));
	virtual CreepProjectile* clone() const;
	~CreepProjectile(void);
//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);
};

