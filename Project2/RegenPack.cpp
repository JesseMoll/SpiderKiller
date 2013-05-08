#include "RegenPack.h"


RegenPack::RegenPack(Drawable* _Parent, double _RegenRate, double _RegenTime):
	Projectile(_Parent),
	RegenRate(_RegenRate),
	RegenTime(_RegenTime),
	ElapsedTime(0)
{
}

RegenPack* RegenPack::clone() const
{
	return new RegenPack(*this);
}

RegenPack::~RegenPack(void)
{
}

void RegenPack::draw2()
{
	//Nothing to draw
}

UpdateResult RegenPack::update2(int ms, GlobalState &GS)
{
	ElapsedTime += ms;
	if(ElapsedTime > RegenTime)
		return UPDATE_DELETE;
	GS.HeroHealth = std::min(GS.HeroMaxHealth, GS.HeroHealth + RegenRate * (double(ms) / 1000.0));
	return UPDATE_REDRAW;
}
