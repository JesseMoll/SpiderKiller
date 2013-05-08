#include "Scene.h"
#include "Hero.h"
#include "creep_manager.h"
#include "texture_manager.h"
#include "weapon_manager.h"
#include <sstream>
#include "RegenPack.h"
#include "Health.h"

//static class variables
Scene* Scene::ptrInstance = NULL; 
std::chrono::time_point<std::chrono::system_clock> Scene::LastUpdate;
GlobalState Scene::GS;

Scene* Scene::Instance()
{
	if(ptrInstance == NULL)
		ptrInstance = new Scene;
	return ptrInstance;
}

Scene::Scene()
{
	LastUpdate = std::chrono::system_clock::now();


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(720, 720);// Initial Window
	glutCreateWindow("Kill the Spiders!!!");

	glutReshapeFunc(Reshape); // window reshape callback
	glutDisplayFunc(Display); // (re)display callback
	glutIdleFunc(Idle); // incremental update
	glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
	glutMouseFunc(mouseButton); // process mouse button push/release
	glutMotionFunc(mouseMove); // process mouse dragging motion
	glutKeyboardFunc(pressNormalKeys); // process standard key pressed
	glutKeyboardUpFunc(releaseNormalKeys); //process standard key release
	glutSpecialFunc(pressSpecialKey); // process special key pressed
	glutSpecialUpFunc(releaseSpecialKey); // process special key release
	
	glutIgnoreKeyRepeat(1);

	GS.TheScene = this;
    
	glutTimerFunc(0, Timer, 0);

}

void Scene::DestructGame()
{
	delete hud;
}

void Scene::InitGame()
{
	SetupTexture();
	new (&GS) GlobalState();
	GS.TheHero = AddChild(new Hero(this, 0, Vector2d(584, LevelSize/2)));	
	GS.TheCreepManager = AddChild(new creep_manager(this));
	GS.TheWeaponManager = AddChild(new weapon_manager(this));
	GS.TheGrid = static_cast<Grid*>(AddChild(new Grid));


	//After we load the textures, init the hud
	hud = new HUD;

	//Get the weapon manager pointer as the correct type
	weapon_manager* WM = static_cast<weapon_manager*>(GS.TheWeaponManager);
	//Add some projectiles
	WM->add_projectile("Bullet",	"", .5, 100, 40, 1e6);
	WM->add_projectile("Pellet",	"", .5, 100, 5,  80); 
	WM->add_projectile("Flame", "Flame.bmp", 4, 100, 3, 50);

	WM->add_projectile("Shell",		"",  .2, 40, 10, 3,  "Pellet",	12,  60);
	WM->add_projectile("Fire",		"",  0, 40, 10, 3,  "Flame",	8,  60); 
	WM->add_projectile("Fire Bomb",	"", .5, 100, 30, 75, "Fire",	8, 360); 
	WM->add_projectile("Grenade",	"", .5, 100, 30, 75, "Shell",	8, 360);
	WM->add_projectile("Mine",	"Mine.bmp", 4, .01, 1, 1, "Shell",	8, 360);
	WM->add_projectile("Super Bomb","", .5, 120, 30, 80, "Fire Bomb", 10, 360);

	//Special Weapons can be initialized like this (specialized for each weapon)
	//Regen 50 health per second for 15 seconds
	WM->add_projectile("Regen", new RegenPack(this,50, 15000));
	//TODO ADD Weapons as we pick them up
	//TODO Separate Weapons as left-click, right-click, and spacebar (super weapons which take energy gained from kills)
	
	
	WM->add_weapon("Machine Gun", "MachineGun.bmp", 100.0, "Bullet");
	WM->add_weapon("Shotgun", "Shotgun.bmp",  1000, "Shell");
	WM->add_weapon("Auto Shotgun", "AutoShotgun.bmp",  300, "Shell");
	WM->add_weapon("Flamethrower", "Flamethrower.bmp",  75.0, "Fire");
	WM->add_weapon("Pistol",  "Pistol.bmp", 500.0, "Bullet");

	WM->add_weapon("Grenade Launcher",  "GrenadeLauncher.bmp", 1000.0, "Grenade", Weapon::EQUIP_RIGHT);
	WM->add_weapon("Fire Bomb Gun", "FireBombGun.bmp",  1000, "Fire Bomb", Weapon::EQUIP_RIGHT);
	WM->add_weapon("BFG", "BFG.bmp",  2000.0, "Super Bomb", Weapon::EQUIP_RIGHT);
	WM->add_weapon("Mine Layer",  "Mine.bmp", 1000.0, "Mine", Weapon::EQUIP_RIGHT);

	WM->add_weapon("Regen Pack", "RegenPack.bmp", 1000.0, "Regen", Weapon::EQUIP_SPACE);
	NextLevel();
}

void Scene::draw2()
{	
	//Turn Textures on for the ground
	
	glBindTexture (GL_TEXTURE_2D, texture_manager::get_texture_name("Level2.bmp"));
	//Draw the ground (10 squares x 10 squares)

	const int NUM_QUADS = 10;
	glPushMatrix();
	glScaled(LevelSize,LevelSize,LevelSize);
	glBegin(GL_QUADS);
		for(int x = 0; x != NUM_QUADS; x++)
		{
			for(int y = 0; y != NUM_QUADS; y++)
			{
				glTexCoord2f(float(x)/float(NUM_QUADS),float(y)/float(NUM_QUADS));
				glVertex2f  (float(x)/float(NUM_QUADS),float(y)/float(NUM_QUADS));

				glTexCoord2f(float(x+1)/float(NUM_QUADS),float(y)/float(NUM_QUADS));
				glVertex2f  (float(x+1)/float(NUM_QUADS),float(y)/float(NUM_QUADS));

				glTexCoord2f(float(x+1)/float(NUM_QUADS),float(y+1)/float(NUM_QUADS));
				glVertex2f  (float(x+1)/float(NUM_QUADS),float(y+1)/float(NUM_QUADS));

				glTexCoord2f(float(x)/float(NUM_QUADS),float(y+1)/float(NUM_QUADS));
				glVertex2f  (float(x)/float(NUM_QUADS),float(y+1)/float(NUM_QUADS));
			}
		}
	glEnd();
	glPopMatrix();
}

void Scene::draw()
{
	int w = static_cast<int>(GS.WindowSize.x);
	int h = static_cast<int>(GS.WindowSize.y);

	const double ViewSizeY = GS.ViewSize;
	const double ViewSizeX = (ViewSizeY * w) / h;

	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	Vector2d CamPos = GS.HeroPos + GS.CameraOffset;
	gluOrtho2D(CamPos.x - ViewSizeX, CamPos.x + ViewSizeX, CamPos.y - ViewSizeY, CamPos.y + ViewSizeY);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glLoadIdentity(); // reset the modelview

	glEnable (GL_TEXTURE_2D);
	Drawable::draw();

	//Draw the HUD over everything else
	hud->draw(static_cast<int>(GS.WindowSize.x), GS);

	glViewport(w - w/4, 0, w / 4, w / 4); // set viewport (drawing area) to entire window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // reset the projection
	gluOrtho2D(0, LevelSize, 0, LevelSize);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glLoadIdentity(); // reset the modelview
	draw2();
	glDisable (GL_TEXTURE_2D);
	GS.TheCreepManager->draw();

	glTranslated(GS.HeroPos.x, GS.HeroPos.y, 0);
	glColor3d(0, 1, 0); //Draw a 2x2 Colored square
	glRectf(-4,-4,4,4);
	
}

void Scene::Timer(int value){

    std::chrono::time_point<std::chrono::system_clock> CurrentTime = std::chrono::system_clock::now();
	__int64 msSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastUpdate).count();
	__int64 microsecSinceLastUpdate = std::chrono::duration_cast<std::chrono::microseconds>(CurrentTime - LastUpdate).count();
	LastUpdate = CurrentTime;

	std::stringstream s;
	s.unsetf ( std::ios::floatfield );
	s.precision(3);
	s << 1000000.0/microsecSinceLastUpdate << " FPS";

	glutSetWindowTitle(s.str().c_str());
	if(GS.KeyStates & PG_DN_KEY)
		GS.ViewSize = std::min(MaxView, exp(msSinceLastUpdate / 1000.0) * GS.ViewSize);
	if(GS.KeyStates & PG_UP_KEY)
		GS.ViewSize = std::max(MinView, exp(-msSinceLastUpdate / 1000.0) * GS.ViewSize);

	if(GS.KeyStates & LEFT_KEY)
		GS.CameraOffset.x -= CameraMoveSpeed * msSinceLastUpdate * GS.ViewSize;
	if(GS.KeyStates & RIGHT_KEY)
		GS.CameraOffset.x += CameraMoveSpeed * msSinceLastUpdate * GS.ViewSize;
	if(GS.KeyStates & UP_KEY)
		GS.CameraOffset.y += CameraMoveSpeed * msSinceLastUpdate * GS.ViewSize;
	if(GS.KeyStates & DOWN_KEY)
		GS.CameraOffset.y -= CameraMoveSpeed * msSinceLastUpdate * GS.ViewSize;
	static __int64 msSinceLastArrowKey = 0;
	//If none of the arrow keys are down, return 
	if(!(GS.KeyStates & (LEFT_KEY | RIGHT_KEY | UP_KEY | DOWN_KEY)))
	{
		msSinceLastArrowKey += msSinceLastUpdate;
		if(msSinceLastArrowKey > 1000)
		{
			GS.CameraOffset.x *= .9;
			GS.CameraOffset.y *= .9;
		}
		
	}
	else
	{
		msSinceLastArrowKey = 0;
	}
	if(GS.KeyStates & TILDE_KEY)
	{
		GS.Debug = !GS.Debug;
		GS.KeyStates &= ~TILDE_KEY;
	}

	//Pause/unpause the game if the R key is pressed
	if (GS.KeyDown(R_KEY))
	{
		GS.KeyStates &= ~R_KEY;
		GS.GamePaused = !GS.GamePaused;
	}

	if (GS.GamePaused)
	{
		glutTimerFunc(UPDATE_INTERVAL, Timer, 0);
		return;
	}
	if(ptrInstance->update((int)msSinceLastUpdate, GS) != UPDATE_NONE) // update everything and if anything has changed...
		glutPostRedisplay(); // redraw everything;
    glutTimerFunc(UPDATE_INTERVAL, Timer, 0);
}


UpdateResult Scene::update2(int ms, GlobalState &GS)
{
	return UPDATE_NONE;
}


	void Scene::pressNormalKeys(unsigned char key, int xx, int yy){
		if(key == 'c' || key == 'C')
			GS.KeyStates |= C_KEY;
		if(key == 'w' || key == 'W')
			GS.KeyStates |= W_KEY;
		if(key == 'a' || key == 'A')
			GS.KeyStates |= A_KEY;
		if(key == 's' || key == 'S')
			GS.KeyStates |= S_KEY;
		if(key == 'd' || key == 'D')
			GS.KeyStates |= D_KEY;
		if(key == 'q' || key == 'Q')
			GS.KeyStates |= Q_KEY;
		if(key == 'e' || key == 'E')
			GS.KeyStates |= E_KEY;
		if(key == 'f' || key == 'F')
			GS.KeyStates |= F_KEY;
		if (key == 'r' || key == 'R')
			GS.KeyStates |= R_KEY;
		if(key == 27)
			GS.KeyStates |= ESC;
		if(key == '`')
			GS.KeyStates |= TILDE_KEY;
		if (key == ' ' || key == ' ')
			GS.KeyStates |= SPACE_KEY;
	}

	void Scene::releaseNormalKeys(unsigned char key, int xx, int yy){
		if(key == 'c' || key == 'C')
			GS.KeyStates &= ~C_KEY;
		if(key == 'w' || key == 'W')
			GS.KeyStates &= ~W_KEY;
		if(key == 'a' || key == 'A')
			GS.KeyStates &= ~A_KEY;
		if(key == 's' || key == 'S')
			GS.KeyStates &= ~S_KEY;
		if(key == 'd' || key == 'D')
			GS.KeyStates &= ~D_KEY;
		if(key == 'q' || key == 'Q')
			GS.KeyStates &= ~Q_KEY;
		if(key == 'e' || key == 'E')
			GS.KeyStates &= ~E_KEY;
		if(key == 'f' || key == 'F')
			GS.KeyStates &= ~F_KEY;
		if(key == 27)
			exit(0);
		if (key == 'r' || key == 'R')
			GS.KeyStates &= ~R_KEY;
		if (key == ' ' || key == ' ')
			GS.KeyStates &= ~SPACE_KEY;

	}

	void Scene::pressSpecialKey(int key, int xx, int yy){
		if(key == GLUT_KEY_PAGE_UP)
			GS.KeyStates |= PG_UP_KEY;
		if(key == GLUT_KEY_PAGE_DOWN)
			GS.KeyStates |= PG_DN_KEY;
		if(key == GLUT_KEY_LEFT)
			GS.KeyStates |= LEFT_KEY;
		if(key == GLUT_KEY_RIGHT)
			GS.KeyStates |= RIGHT_KEY;
		if(key == GLUT_KEY_UP)
			GS.KeyStates |= UP_KEY;
		if(key == GLUT_KEY_DOWN)
			GS.KeyStates |= DOWN_KEY;

	}
	void Scene::releaseSpecialKey(int key, int xx, int yy){
		if(key == GLUT_KEY_PAGE_UP)
			GS.KeyStates &= ~PG_UP_KEY;
		if(key == GLUT_KEY_PAGE_DOWN)
			GS.KeyStates &= ~PG_DN_KEY;
		if(key == GLUT_KEY_LEFT)
			GS.KeyStates &= ~LEFT_KEY;
		if(key == GLUT_KEY_RIGHT)
			GS.KeyStates &= ~RIGHT_KEY;
		if(key == GLUT_KEY_UP)
			GS.KeyStates &= ~UP_KEY;
		if(key == GLUT_KEY_DOWN)
			GS.KeyStates &= ~DOWN_KEY;
	}

	void Scene::mouseMove(int xx, int yy){
		GS.MousePos = Vector2d(xx,yy);
	}

	void Scene::mouseButton(int button, int state, int xx, int yy){
		//Register left mouse as a keystate
		if(button == 0)
		{
			if(!state)
				GS.KeyStates |= LEFT_MOUSE;
			else
				GS.KeyStates &= ~LEFT_MOUSE;
		}
		if(button == 2)
		{
			if(!state)
				GS.KeyStates |= RIGHT_MOUSE;
			else
				GS.KeyStates &= ~RIGHT_MOUSE;
		}
		GS.MousePos = Vector2d(xx,yy);
	}
	
	void Scene::Display()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // clear buffers
		ptrInstance->draw(); // draw the world
		glutSwapBuffers(); // make it all visible
	}

	void Scene::Reshape(int w, int h)
	{
		static bool FirstTime = true;
		if(FirstTime)
		{
			FirstTime = false;

			//Allow transparency in textures
			glEnable (GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

			//Init everything after textures and whatnot are setup
			ptrInstance->InitGame();
		}

		GS.WindowSize = Vector2d(w,h);
	}

	void Scene::Idle()
	{
		
	}

	void Scene::NextLevel()
	{
		GS.CurrentLevel++;
		if (GS.CurrentLevel == 6)
			GS.CurrentLevel = 1;

		switch (GS.CurrentLevel)
		{
		case 1:
			//Get the creep manager pointer as the correct type
			creep_manager* CM = static_cast<creep_manager*>(GS.TheCreepManager);
			CM->add_creep("Tiny Spider", 2, "Spider.bmp", 1.5, 50, 5);
			CM->add_creep("Medium Spider", 10, "Spider.bmp", 4, 40, 4);
			CM->add_creep("Huge Spider", 500, "Spider.bmp", 10, 20, 2);

			//TODO - Move this code into the level class (on level init)
			//TODO, add finite spawns (so we can beat a level)

			//Add some spawners 
			creep_spawner* CS1 = CM->add_spawner(Vector2d(400,674), 5000, 200, 0, "Tiny Spider");
			creep_spawner* CS2 = CM->add_spawner(Vector2d(400,674), 5000, 10, 200, "Medium Spider");
			//remove the spawner
			//CS2->remove();
			//change the spawn amount and spawn rate of the spawner
			CS1->setSpawnAmount(20);
			CS1->setSpawnRate(500);
			//Will delete itself after spawning 200 more creep
			CS1->setSpawnLimit(200);
			
			CM->add_spawner(Vector2d(400,674), 20000, 1, 2, "Huge Spider");

			//Add a health pack
			AddChild(new Health(this, Vector2d(595, LevelSize/2)));
		}
	}