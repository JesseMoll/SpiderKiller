#pragma once

#include <list>
#include "GlobalState.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include "Vector2d.h"


class Drawable
{
    public:
		//constructor (Takes position and parent, leave inheritance to handle anything else)
        Drawable(Drawable* _Parent = 0, GLuint _Texture = 0, Vector2d _Pos = Vector2d(0,0), Vector2d _Scale = Vector2d(1,1), Vector3d _Color = Vector3d(1,1,1));

		//deconstructor (virtual because it will handle all cleanup)
        virtual ~Drawable();

		// Draw Function (including parent-to-child transformation)
        virtual void draw(); 

		//Update function and its children
		virtual UpdateResult update(int ms, GlobalState &GS); 

		//Update function (position, state, etc...)
		virtual UpdateResult update2(int ms, GlobalState &GS); 
		
		//Add a child to this drawable
		Drawable* AddChild(Drawable* dPtr);

		//Accessor for the Position
		Vector2d getPos();

		//Accessor for the Position
		inline Vector2d getLocalPos(){return Pos;};

		//Accessor for the Position
		inline GLuint getTexture(){return Texture;};

		//Accessor for the Rotation
		double getRot(bool GlobalRot = false);

		//Mutator for the Position
		void setPos(Vector2d _Pos){Pos = _Pos;};

		//Mutator for the Rotation
		void setRot(double _Rot){Rot = _Rot;};

		//Gets the bounding rectangle for this drawable (including all children)
		virtual Rect2d GetBoundingRect();

		//Gets the bounding rectangle for this drawable (excluding all children)
		virtual Rect2d GetBoundingRect2();

		//Expose the begining and end of the children so that we can iterate through a drawable's children
		std::list<Drawable*>::iterator begin(){return Children.begin();};
		std::list<Drawable*>::iterator end(){return Children.end();};

		
   // protected:

		virtual void draw2(); // Draw the object (excluding parent-to-child transformation)
        Vector2d Pos;	// localized object Position
		Vector2d Scale;	// Object's Scale
		Vector3d Color; // Object's color
		GLuint Texture; // Texture of the Object (if 0, use the color)
		double Rot;
		Drawable * Parent; //Parent Drawable
		std::list<Drawable *> Children; //List of Children

};
