#include "HUD.h"


HUD::HUD(void)
{
	Texture = texture_manager::get_texture_name("HUD.bmp");
}

void HUD::draw(int window_width)
{

	glViewport(0, 0, window_width, window_width / 4); // set viewport (drawing area) to entire window

	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluOrtho2D(0,4,0,1);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glLoadIdentity(); // reset the modelview
	glEnable (GL_TEXTURE_2D);


		glBindTexture (GL_TEXTURE_2D, Texture);
		
		glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f  (0,0);

				glTexCoord2f(1,0);
				glVertex2f  (4,0);

				glTexCoord2f(1,1);
				glVertex2f  (4,1);

				glTexCoord2f(0,1);
				glVertex2f  (0,1);


	glEnd();
}

HUD::~HUD(void)
{
}
