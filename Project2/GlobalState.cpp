#include "GlobalState.h"




//default constructor (set up all the global variables)
GlobalState::GlobalState(void):
	KeyStates(0),
	CamPos(10,10),
	View(VIEW_TOP_DOWN),
	MousePos(0,0),
	MouseSemaphore(false),
	MoveSpeed(.5)
{
}


GlobalState::~GlobalState(void)
{
}
