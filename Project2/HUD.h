#pragma once

#include "texture_manager.h"
#include <GL/glut.h>

class HUD
{
public:
	HUD(void);
	virtual void draw(int window_width);
	~HUD(void);
protected:
	GLuint Texture; // Texture of the Object (if 0, use the color)
};

