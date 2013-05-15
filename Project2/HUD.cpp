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


	Weapon* LeftWeapon = WM->getEquippedWeapon(Weapon::EQUIP_LEFT);
	Weapon* RightWeapon = WM->getEquippedWeapon(Weapon::EQUIP_RIGHT);
	Weapon* SuperWeapon = WM->getEquippedWeapon(Weapon::EQUIP_SPACE);

	//Draw the health bar
	double HealthPct = GS.HeroHealth / GS.HeroMaxHealth;
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glColor4f(0,.7,0,.7); //Semi-transparent green
		glTranslatef(384,43,0);
		glScaled(HealthPct * 256,14,0);
		glRectd(0,0,1,1);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	//Draw the focus bar
	double FocusPct = GS.HeroFocus / GS.HeroMaxFocus;
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glColor4f(0,0,.7,.7); //Semi-transparent blue
		glTranslatef(384,70,0);
		glScaled(FocusPct * 256,14,0);
		glRectd(0,0,1,1);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	//Draw HUD background
	glBindTexture (GL_TEXTURE_2D, Texture);	
	glPushMatrix();
		glScalef(1024,256,1);
		glTranslatef(0,0,0);
		DrawTexturedSquare();
	glPopMatrix();

	//Draw LEFT weapon icon
	if(LeftWeapon)
	{
		glBindTexture (GL_TEXTURE_2D, LeftWeapon->getTexture());	
		glPushMatrix();
			glTranslatef(288, 48, 0);
			glScalef(64,64,1);
			DrawTexturedSquare();
		glPopMatrix();
	}

	//Draw Right weapon icon
	if(RightWeapon)
	{
		glBindTexture (GL_TEXTURE_2D, RightWeapon->getTexture());		
		glPushMatrix();
			glTranslatef(672, 48, 0);
			glScalef(64,64,1);
			DrawTexturedSquare();
		glPopMatrix();
	

		double CoolDownPct = RightWeapon->getCoolDown();	
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
			glColor4f(0,.3,.3,.3); //Semi-transparent blue
			glTranslatef(672,48,0);
			glScaled(64,CoolDownPct * 64,0);
			glRectd(0,0,1,1);
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}

	if(SuperWeapon)
	{
		//Draw Super weapon icon
		glBindTexture (GL_TEXTURE_2D, SuperWeapon->getTexture());	
		glPushMatrix();
			glTranslatef(32,64,0);
			glScalef(64,64,1);
			DrawTexturedSquare();
		glPopMatrix();
	}
}

HUD::~HUD(void)
{
}
