#include "Shield.h"
#include "texture_manager.h"

Shield::Shield(Drawable* _Parent, double _Health, double _DecayRate):
	Projectile(_Parent,texture_manager::get_texture_name("Shield.bmp"), 12, 0, _Health),
	DecayRate(_DecayRate)
{
}

Shield* Shield::clone() const
{
	return new Shield(*this);
}

Shield::~Shield(void)
{
}

void Shield::draw2()
{
	Color = Vector4d(1, 1, 1, Damage / 500.0);
	glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	Drawable::draw2();
	glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

UpdateResult Shield::update2(int ms, GlobalState &GS)
{
	Pos = GS.HeroPos;
	Damage -= (ms * DecayRate) / 1000.0;
	if(Damage <= 0)
		return UPDATE_DELETE;
	return Projectile::update2(ms, GS);
}
