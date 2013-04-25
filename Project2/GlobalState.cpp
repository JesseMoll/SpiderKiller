#include "GlobalState.h"




//default constructor (set up all the global variables)
GlobalState::GlobalState(void):
	KeyStates(0),
	HeroPos(10,10),
	MousePos(0,0),
	MouseSemaphore(false),
	MoveSpeed(30)
{
	
}


GlobalState::~GlobalState(void)
{
}
