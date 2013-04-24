#include "Bullet.h"


Bullet::Bullet(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, double _Scale, double _Speed, Vector3d _Color):
	Drawable(_Parent, _Texture, _Pos, Vector2d(_Scale,_Scale), _Color),
	Speed(_Speed)
{
}

Bullet::Bullet(Bullet * c):
	Drawable(c->Parent, c->Texture, c->Pos, c->Scale, c->Color),
	Speed(c->Speed)
{
}

Bullet::~Bullet(void)
{
}

UpdateResult Bullet::update2(int ms, GlobalState &GS)
{
	return UPDATE_REDRAW;
}