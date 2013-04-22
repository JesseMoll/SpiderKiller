#pragma once
#include "Drawable.h"

class Hero :
	public Drawable
{
public:
	Hero(Drawable* _Parent, GLuint _Texture, Vector2d _Pos);
	~Hero(void);
	UpdateResult update2(int ms, GlobalState &GS);
protected:
	float AnimationTime;
};
