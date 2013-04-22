#pragma once
#include "Drawable.h"

class Hero :
	public Drawable
{
public:
	Hero(Vector2d _Pos, Drawable* _Parent, GLuint _Texture);
	~Hero(void);
	UpdateResult update2(int ms, GlobalState &GS);
protected:
	float AnimationTime;
};
