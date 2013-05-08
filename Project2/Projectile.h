#pragma once
#include "drawable.h"
class Projectile :
	public Drawable
{
public:
	Projectile(Drawable* _Parent, GLuint _Texture = 0, double _Scale = .2, double _Speed = 40, double _Damage = 100, double _MaxDistance = 1e6, Projectile* _ProjectileToFireOnDeath = 0, int _NumToFireOnDeath = 1,  double _SpreadOnDeath = 0, Vector3d _Color = Vector3d(1,1,1));
	~Projectile(void);

//Inherited Functions
	virtual UpdateResult update2(int ms, GlobalState &GS);
	virtual Projectile* clone() const;

protected:
	void onDeath(GlobalState &GS);
	double Speed;
	double Damage;
	double MaxDistance;
	Projectile* ProjectileToFireOnDeath;
	int NumToFireOnDeath;
	double SpreadOnDeath;
	double DistanceTravelled;
};