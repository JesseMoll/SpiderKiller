#pragma once

#include "Vector2d.h"
#include "Util.h"
#include <GL/glut.h>


class Drawable;




enum UpdateResult
{
	UPDATE_NONE,
	UPDATE_REDRAW,
	UPDATE_DELETE
};


const unsigned int LEFT_MOUSE = 0x1;
const unsigned int RIGHTKEY = 0x2;
const unsigned int C_KEY = 0x4;
const unsigned int Q_KEY = 0x8;
const unsigned int W_KEY = 0x10;
const unsigned int ESC = 0x20;
const unsigned int A_KEY = 0x40;
const unsigned int S_KEY = 0x80;
const unsigned int D_KEY = 0x100;

const unsigned int UPDATE_INTERVAL = 10;


class GlobalState
{
public:
	GlobalState();
	~GlobalState(void);
	bool KeyDown(unsigned int Key)
	{
		return ((KeyStates & Key) != 0);
	}
	//The hero
	Drawable *TheHero;
	//Root of the scene graph
	Drawable *TheScene;	
	//For accessing the creeps
	Drawable *TheCreepManager;
	Drawable *TheWeaponManager;

	//Each key is represented by a bit, 1 means down, 0 means up
	unsigned int KeyStates;	

	//Camera values

	Vector2d HeroPos;

	Vector2d WindowSize;

	//Mouse Position on button down
	Vector2d MousePos;

	//To keep the mouse reposition from calling itself
	volatile bool MouseSemaphore;

	double MoveSpeed;

	GLuint Tex;
	

};
