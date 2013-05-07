#pragma once

#include "Vector2d.h"
#include "Util.h"
#include <GL/glut.h>

class Grid;
class Drawable;




enum UpdateResult
{
	UPDATE_NONE,
	UPDATE_REDRAW,
	UPDATE_DELETE
};


const unsigned int LEFT_MOUSE = 0x1;
const unsigned int RIGHT_MOUSE = 0x2;
const unsigned int C_KEY = 0x4;
const unsigned int Q_KEY = 0x8;
const unsigned int W_KEY = 0x10;
const unsigned int ESC = 0x20;
const unsigned int A_KEY = 0x40;
const unsigned int S_KEY = 0x80;
const unsigned int D_KEY = 0x100;
const unsigned int PG_UP_KEY = 0x200;
const unsigned int PG_DN_KEY = 0x400;
const unsigned int RIGHT_KEY = 0x800;
const unsigned int LEFT_KEY = 0x1000;
const unsigned int UP_KEY = 0x2000;
const unsigned int DOWN_KEY = 0x4000;
const unsigned int TILDE_KEY = 0x8000;
const unsigned int R_KEY = 0x10000;



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
	Grid* TheGrid;
	//Each key is represented by a bit, 1 means down, 0 means up
	unsigned int KeyStates;	

	//Camera values
	Vector2d CameraOffset;
	double ViewSize;

	//Hero Values
	Vector2d HeroPos;
	double HeroMaxHealth;
	double HeroHealth;
	double HeroFocus;
	double HeroMaxFocus;
	double HeroRegenPerSec;


	Vector2d WindowSize;
	Vector2d MousePos;

	double MoveSpeed;

	//Generalized debug var
	bool Debug;

	//GLuint Tex;
	GLuint LeftWeaponTex;
	GLuint RightWeaponTex;
	GLuint SuperWeaponTex;

	bool GamePaused;

};
