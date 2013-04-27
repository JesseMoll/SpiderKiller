#pragma once
#include "Drawable.h"
#include "Weapon.h"
#include <map>
#include <string>

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
