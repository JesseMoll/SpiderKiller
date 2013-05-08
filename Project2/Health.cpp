#include "Health.h"
#include "texture_manager.h"
#include <algorithm>
#include <iostream>

Health::Health(Drawable* _Parent, Vector2d _Pos):
	Drawable(_Parent, texture_manager::get_texture_name("health.bmp"), _Pos, Vector2d(5, 5)),
	PrevTexture(0),
	RespawnTime(5000),
	HealthAmount(100)
{
	RespawnClock = RespawnTime;
	DrawColoredRect = false;
}

UpdateResult Health::update2(int ms, GlobalState &GS)
{
	//Check if we're invisible and need to respawn
	if (Texture == 0)
	{
		RespawnClock -= ms;
		std::cout<<RespawnClock<<std::endl;
		if (RespawnClock <= 0)
		{
			RespawnClock += RespawnTime;
			std::swap(Texture, PrevTexture);
			return UPDATE_REDRAW;
		}

		return UPDATE_REDRAW;
	}

	//Check if the Hero has collided with us and we need to replenish his/her health
	Rect2d HealthRect = GetBoundingRect();
	Rect2d HeroRect = GS.TheHero->GetBoundingRect();
	if (HealthRect.overlaps(HeroRect))
	{
		//Add health to hero and disappear until RespawnTime has passed
		if (GS.HeroHealth + HealthAmount > GS.HeroMaxHealth)
			GS.HeroHealth = GS.HeroMaxHealth;
		else GS.HeroHealth += HealthAmount;
		std::swap(Texture, PrevTexture);
	}

	return UPDATE_REDRAW;
}