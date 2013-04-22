#pragma once
#include "drawable.h"
class Creep :
	public Drawable
{
public:
	Creep(Creep* c);
	Creep(Drawable* _Parent, GLuint _Texture = 0, Vector2d _Pos = Vector2d(0,0), double Scale = .5, double _Speed = .25, double _TurnSpeed = 5, Vector3d _Color = Vector3d(1,1,1));
	~Creep(void);

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	double Speed;
	double TurnSpeed;
};

