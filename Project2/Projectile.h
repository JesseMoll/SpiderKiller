#pragma once
#include "drawable.h"
class Projectile :
	public Drawable
{
public:
	Projectile(Projectile* c, Vector2d _Pos, double _Rot = 0);
	Projectile(Drawable* _Parent, GLuint _Texture = 0, double _Scale = .2, double _Speed = 40, double _Damage = 100, double _MaxDistance = 1e6, Projectile* _ProjectileToFireOnDeath = 0, int _NumToFireOnDeath = 1,  double _SpreadOnDeath = 0, Vector3d _Color = Vector3d(1,1,1));
	~Projectile(void);

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);

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