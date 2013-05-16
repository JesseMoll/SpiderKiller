#pragma once
#include "drawable.h"
#include <string>

class MessageScreen :
	public Drawable
{
public:
	MessageScreen(Drawable* _Parent = 0, Vector2d _Pos = Vector2d(0,0));
	void draw2();
	UpdateResult update2(int ms, GlobalState &GS);
	//Sets what is said when the message shown
	void setMessage(std::string NewMessage){Message = NewMessage;};
private:
	std::string Message;
};

