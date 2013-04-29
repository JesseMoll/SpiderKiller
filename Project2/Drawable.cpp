#include "Drawable.h"
#include <iostream>



Drawable::Drawable(Drawable* _Parent, GLuint _Texture, Vector2d _Pos, Vector2d _Scale, Vector3d _Color):
	Pos(_Pos),
	Parent(_Parent),
	Texture(_Texture),
	Scale(_Scale),
	Color(_Color),
	Rot(0)
{
}


Drawable::~Drawable()
{
	//clean up all the children
	for (std::list<Drawable*>::iterator dPtr = Children.begin();dPtr != Children.end(); ++dPtr) {
		delete (*dPtr);
	}
}

//Returns the actual (not relative to the parent) position of the object
Vector2d Drawable::getPos()
{
	Vector2d RetVal = Pos;
	if(Parent != 0)
		RetVal += Parent->getPos();
	return RetVal;
};

//Returns the actual (not relative to the parent) position of the object
double Drawable::getRot(bool GlobalRot)
{

	double RetVal = Rot;
	if (GlobalRot && Parent != 0)
	{
		RetVal += Parent->getRot(true);
	}
	return RetVal;
};

//Will get the relative bounding rectangle of the object itself (not including children)
//this can be overridden in case of non 2x2 squares
Rect2d Drawable::GetBoundingRect2()
{
	//by default (all squares drawn in this program) squares are all from -1,-1 to 1,1
	return Rect2d(Point2d(-1,-1) * Scale + Pos, Point2d( 1, 1) * Scale + Pos);
	//return Rect2d(Point2d(-1,-1), Point2d( 1, 1));
}

//Will get the bounding rectangle of the object and its children
Rect2d Drawable::GetBoundingRect()
{
	//gets the bounding rect of the object itself
	Rect2d RetVal = GetBoundingRect2();
	//If it has a parent, convert relative to absolute coords
	if(Parent != 0)
	{
		RetVal += Parent->getPos();
	}
	//Go through all the children and perform a Union of all the bounding rects
	for (std::list<Drawable*>::iterator dPtr = Children.begin();dPtr != Children.end(); ++dPtr) {
		RetVal = mergeBounds(RetVal, (*dPtr)->GetBoundingRect());
	}
	//return a bounding rect encompasing all children
	return RetVal;
}

void Drawable::draw()
{
	glPushMatrix();
		glTranslated(Pos.x, Pos.y, 0); // translate to Drawable's location
		glPushMatrix();
			//Draw the scaled object
			glScaled(Scale.x, Scale.y, 1.0f);
			glRotated(RadToDeg(Rot), 0, 0, 1);
			draw2();
		glPopMatrix();
		//Draw all the children
		for (std::list<Drawable*>::iterator dPtr = Children.begin();dPtr != Children.end(); ++dPtr) {
			(*dPtr)->draw();
		}
	glPopMatrix();
}

void Drawable::draw2()
{
	if(Texture == 0)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3d(Color.x, Color.y, Color.z); //Draw a 2x2 Colored square
		glRectf(-1,-1,1,1);
		glRectf(-.25f,-.25f,2,.25f);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glColor3d(Color.x, Color.y, Color.z);
		glBindTexture (GL_TEXTURE_2D, Texture);
		
		glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f  (-1,-1);

				glTexCoord2f(1,0);
				glVertex2f  (1,-1);

				glTexCoord2f(1,1);
				glVertex2f  (1,1);

				glTexCoord2f(0,1);
				glVertex2f  (-1,1);
	glEnd();
	}

}

UpdateResult Drawable::update(int ms, GlobalState &GS)
{
	//Update the object itself
	UpdateResult RetVal = update2(ms, GS);
	//If the object is to be deleted, no need to handle children, just return
	if(RetVal == UPDATE_DELETE)
		return RetVal;

	//iterate through the list (not standard for loop because we erase from within)
	std::list<Drawable*>::iterator dPtr = Children.begin();
	while(dPtr != Children.end()){
		UpdateResult ChildResult = (*dPtr)->update(ms, GS);
		switch (ChildResult)
		{
		case UPDATE_NONE:
			//nothing special, just go to the next child
			++dPtr;
			break;
		case UPDATE_DELETE:
			//The child needs to be erased, no need to increment the iterator, the erasing will
			//move it to the next item
			delete *dPtr;
			dPtr = Children.erase(dPtr);
			//We need to redraw after erasing
			RetVal = UPDATE_REDRAW;
			break;
		case UPDATE_REDRAW:
			//Child need to redraw, so pass the message back
			RetVal = UPDATE_REDRAW;
			++dPtr;
		}
	}
	return RetVal;
}

UpdateResult Drawable::update2(int ms, GlobalState &GS)
{
	//by default, things don't need to be updated
	return UPDATE_NONE;
}

Drawable* Drawable::AddChild(Drawable* dPtr)
{
	Children.push_back(dPtr);
	return dPtr;
}
