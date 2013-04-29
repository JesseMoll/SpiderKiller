#include "HUD.h"
#include "weapon_manager.h"

HUD::HUD(void)
{
	Texture = texture_manager::get_texture_name("HUD.bmp");
}

void DrawTexturedSquare()
{
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f  (0,0);

		glTexCoord2f(1,0);
		glVertex2f  (1,0);

		glTexCoord2f(1,1);
		glVertex2f  (1,1);

		glTexCoord2f(0,1);
		glVertex2f  (0,1);
	glEnd();
}
void HUD::draw(int window_width, GlobalState &GS)
{
	weapon_manager* WM = static_cast<weapon_manager*>(GS.TheWeaponManager);
	glViewport(0, 0, window_width, window_width / 4); // set viewport (drawing area) to entire window

	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluOrtho2D(0,1024 ,0,256);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glLoadIdentity(); // reset the modelview


	glBindTexture (GL_TEXTURE_2D, Texture);	
	glPushMatrix();
		glScalef(1024,256,1);
		glTranslatef(0,0,0);
		DrawTexturedSquare();
	glPopMatrix();

	glBindTexture (GL_TEXTURE_2D, WM->getWeaponIcon(Weapon::EQUIP_LEFT));	
	glPushMatrix();
		glTranslatef(288, 48, 0);
		glScalef(64,64,1);
		DrawTexturedSquare();
	glPopMatrix();

	/*
	glBindTexture (GL_TEXTURE_2D, WM->getWeaponIcon(Weapon::EQUIP_RIGHT));		
	glPushMatrix();
		glTranslatef(672, 48, 0);
		glScalef(64,64,1);
		DrawTexturedSquare();
	glPopMatrix();

	glBindTexture (GL_TEXTURE_2D, WM->getWeaponIcon(Weapon::EQUIP_SPACE));	
	glPushMatrix();
		glTranslatef(32,64,0);
		glScalef(64,64,1);
		DrawTexturedSquare();
	glPopMatrix();
	*/
}

HUD::~HUD(void)
{
}
