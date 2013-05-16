#include "MessageScreen.h"

MessageScreen::MessageScreen(Drawable* _Parent,  Vector2d _Pos):Drawable(_Parent, 0, _Pos) 
{
}

void MessageScreen::draw2()
{

		glColor3f(1.0, 1.0, 1.0); 
		glScaled(.025, .025, 1.0f);
		const char* mPtr = Message.c_str();
		glPushMatrix();
		while(*mPtr != 0)
		{
			if(*mPtr == '\n')
			{
				mPtr++;
				glPopMatrix();
				glTranslated(0,-200,0);
				glPushMatrix();
			}
			else
			{
				glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *(mPtr++));
			}
		}
		glPopMatrix();
}

UpdateResult MessageScreen::update2(int ms, GlobalState &GS)
{
	return UPDATE_NONE;
}