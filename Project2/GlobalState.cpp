#include "GlobalState.h"




//default constructor (set up all the global variables)
GlobalState::GlobalState(void):
	KeyStates(0),
	HeroPos(10,10),
	MousePos(0,0),
	MoveSpeed(60),
	TheHero(0),
	TheScene(0),
	TheCreepManager(0),
	TheWeaponManager(0),	
	HeroMaxHealth(1000),
	HeroHealth(1000),
	HeroFocus(0),
	HeroMaxFocus(100),
	HeroRegenPerSec(25.0),
	CameraOffset(0,0),
	ViewSize(100),
	Debug(false)
{
	
}


GlobalState::~GlobalState(void)
{
}
