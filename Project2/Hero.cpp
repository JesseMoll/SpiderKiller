#include "Hero.h"
#include "Projectile.h"
#include "texture_manager.h"
#include "Scene.h"

Hero::Hero(Drawable* _Parent, GLuint _Texture, Vector2d _Pos):Drawable(_Parent, _Texture, _Pos,Vector2d(6,6)),
	AnimationTime(0), ExplosionTimer(1000)
{
	Color = Vector3d(0,1,0);
}

UpdateResult Hero::update2(int ms, GlobalState &GS)
{ 
	GS.HeroHealth = std::min(GS.HeroMaxHealth, GS.HeroHealth + GS.HeroRegenPerSec * (double(ms) / 1000.0));

	Vector2d ViewDirection = GS.MousePos - GS.WindowSize / 2;
	Rot = atan2(-ViewDirection.y, ViewDirection.x);

	//Check to see if health has reached 0, meaning game over
	//TODO:  go back to title screen.  May want to move this check to another place.
	if (GS.HeroHealth <= 0)
	{
		Texture = texture_manager::get_texture_name("Flame.bmp");
		ExplosionTimer -= ms;
		if (ExplosionTimer <= 666)
			Scale = Vector2d(8, 8);
		if (ExplosionTimer <= 333)
			Scale = Vector2d(12, 12);
		if (ExplosionTimer <= -333)
		{
			exit(0); //TODO draw a game over screen
		}

		return UPDATE_REDRAW;
	}

	Vector2d WalkingDirection(0,0);

	//Set walking direction (forward/backward) and angle depending on which key is pressed
	if (GS.KeyDown(W_KEY))
	{
		WalkingDirection.y = 1;
	}
	if (GS.KeyDown(S_KEY))
	{
		WalkingDirection.y = -1;
	}
	if (GS.KeyDown(D_KEY))
	{
		WalkingDirection.x = 1;
	}
	if (GS.KeyDown(A_KEY))
	{
		WalkingDirection.x = -1;
	}

	//If we are walking
	if (WalkingDirection.x != 0 || WalkingDirection.y != 0)
	{
		double NewAngle = (atan2(WalkingDirection.y,WalkingDirection.x));

		//Walk in the direction that he is facing
		Vector2d PosAdder(cos(NewAngle), sin(NewAngle));
		PosAdder *= (ms / 1000.0) * GS.MoveSpeed;
		//Will the new position be valid?
		if(GetWalkable(Pos + PosAdder))
			Pos += PosAdder;
		
		//Update the walking animation
		AnimationTime += ms / 200.0f;

	}
	GS.HeroPos = Pos;
	return UPDATE_REDRAW;
}



Hero::~Hero(void)
{
	//Clean up weapons

}
