#pragma once
#include "drawable.h"
class Creep :
	public Drawable
{
public:
	Creep(Creep* c, Vector2d _Pos = Vector2d(0,0), double _Rot = 1e6);
	Creep(Drawable* _Parent, GLuint _Texture = 0, Vector2d _Pos = Vector2d(0,0), double _Health = 10, double _Scale = .5, double _Rot = 0, double _Speed = .25, double _TurnSpeed = 5, Vector3d _Color = Vector3d(1,0,0));
	~Creep(void);
	bool inline checkOverlap(Vector2d &_Pos, double &Radius)
	{
		return ((Pos-_Pos).length_squared() < (Radius + Scale.x) * (Radius + Scale.x));
	}
	double Damage(double dmg);

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	double Speed;
	double TurnSpeed;
	double Health;
};

