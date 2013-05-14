#pragma once
#include "drawable.h"
class Creep :
	public Drawable
{
public:
	
	Creep(Drawable* _Parent, GLuint _Texture = 0, Vector2d _Pos = Vector2d(0,0), double _Health = 10, double _Scale = .5, double _Rot = 0, double _Speed = .25, double _TurnSpeed = 5, Vector4d _Color = Vector4d(1,0,0,1),	Creep* _SpawnOnDeath = NULL, Creep* _SpawnPeriodic = NULL, double _PeriodicSpawnRate = 1000, double _PeriodicSpawnNum = 0, double _OnDeathSpawnNum = 0);
	virtual Creep* clone() const;
	~Creep(void);
	bool inline checkOverlap(Vector2d &_Pos, double &Radius)
	{
		return ((Pos-_Pos).length_squared() < (Radius + Scale.x) * (Radius + Scale.x));
	}
	double Damage(double dmg);

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	Creep* SpawnOnDeath;
	Creep* SpawnPeriodic;
	double PeriodicSpawnRate;
	double PeriodicSpawnTimer;
	double PeriodicSpawnNum;
	double OnDeathSpawnNum;

	double Speed;
	double TurnSpeed;
	double Health;
};

