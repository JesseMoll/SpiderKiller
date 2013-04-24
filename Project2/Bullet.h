#pragma once
#include "drawable.h"
class Bullet :
	public Drawable
{
public:
	Bullet(Bullet* c);
	Bullet(Drawable* _Parent, GLuint _Texture = 0, Vector2d _Pos = Vector2d(0,0), double Scale = .5, double _Speed = .25, Vector3d _Color = Vector3d(1,1,1));
	~Bullet(void);

//Inherited Functions
	UpdateResult update2(int ms, GlobalState &GS);

protected:
	double Speed;
};