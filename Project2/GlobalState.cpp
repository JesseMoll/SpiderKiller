#include "GlobalState.h"




//default constructor (set up all the global variables)
GlobalState::GlobalState(void):
	KeyStates(0),
	HeroPos(10,10),
	MousePos(0,0),
	MoveSpeed(30),
	TheHero(0),
	TheScene(0),
	TheCreepManager(0),
	TheWeaponManager(0)
{
	
}


GlobalState::~GlobalState(void)
{
}
