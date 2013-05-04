#include "Scene.h"
#include "Hero.h"
#include "creep_manager.h"
#include "texture_manager.h"
#include "weapon_manager.h"
#include <sstream>

const int TextureWidth = 512;
// Texture is a WxWx4 array (rgba)
static GLubyte TexImage[TextureWidth][TextureWidth][4];

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
    GS.TheHero = AddChild(new Hero(this, 0, Vector2d(TextureWidth/2, TextureWidth/2)));	
	GS.TheCreepManager = AddChild(new creep_manager(this));
	GS.TheWeaponManager = AddChild(new weapon_manager(this));
	GS.TheGrid = static_cast<Grid*>(AddChild(new Grid));


	//After we load the textures, init the hud
	hud = new HUD;

	//Get the weapon manager pointer as the correct type
	weapon_manager* WM = static_cast<weapon_manager*>(GS.TheWeaponManager);
	//Add some projectiles
	WM->add_projectile("Bullet",	"", .2, 40, 50, 1e6);
	WM->add_projectile("Pellet",	"", .2, 40, 20,  30); 
	WM->add_projectile("Flame", "Flame.bmp", 2, 40, 6, 20);

	WM->add_projectile("Shell",		"",  .2, 20, 100, 3,  "Pellet",	8,  40);
	WM->add_projectile("Fire",		"",  0, 20, 100, 3,  "Flame",	8,  60); 
	WM->add_projectile("Fire Bomb",	"", .5, 30, 100, 15, "Fire",	8, 360); 
	WM->add_projectile("Grenade",	"", .5, 30, 100, 15, "Shell",	8, 360);
	WM->add_projectile("Super Bomb","", .5, 80, 100, 30, "Fire Bomb", 10, 360);
	//TODO ADD Weapons as we pick them up
	//TODO Separate Weapons as left-click, right-click, and spacebar (super weapons which take energy gained from kills)
	WM->add_weapon("Machine Gun", "MachineGun.bmp", 100.0, "Bullet");
	WM->add_weapon("Shotgun", "Shotgun.bmp",  750, "Shell");
	WM->add_weapon("Auto Shotgun", "AutoShotgun.bmp",  300, "Shell");
	WM->add_weapon("Flamethrower", "Flamethrower.bmp",  75.0, "Fire");
	WM->add_weapon("Grenade Launcher",  "GrenadeLauncher.bmp", 1000.0, "Grenade");
	WM->add_weapon("Fire Bomb Gun", "FireBombGun.bmp",  1000, "Fire Bomb");
	WM->add_weapon("BFG", "BFG.bmp",  2000.0, "Super Bomb");
	WM->add_weapon("Pistol",  "Pistol.bmp", 250.0, "Bullet");

	//Get the creep manager pointer as the correct type
	creep_manager* CM = static_cast<creep_manager*>(GS.TheCreepManager);
	CM->add_creep("Tiny Spider", 5, "Spider.bmp", .75, 25, 5);
	CM->add_creep("Small Spider", 10, "Spider.bmp", 1.5, 25, 4);
	CM->add_creep("Huge Spider", 500, "Spider.bmp", 10, 10, 2);

	//TODO - Move this code into the level class (on level init)
	//TODO, add finite spawns (so we can beat a level)
	CM->add_spawner(Vector2d(225,300), 500, 10, "Tiny Spider",  270);
	CM->add_spawner(Vector2d(250,200), 500, 10, "Small Spider",  90);
	//CM->add_spawner(Vector2d(250,200), 10000, 1, "Huge Spider",  90);
}

void Scene::draw2()
{	
	//Turn Textures on for the ground
	
	glBindTexture (GL_TEXTURE_2D, texture_manager::get_texture_name("Level1.bmp"));
	//Draw the ground (10 squares x 10 squares)

	const int NUM_QUADS = 10;
	glPushMatrix();
	glScaled(TextureWidth,TextureWidth,TextureWidth);
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

	

    
	
	const int ViewSizeY = 60;
	const int ViewSizeX = (60 * w) / h;

	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluOrtho2D(GS.HeroPos.x - ViewSizeX, GS.HeroPos.x + ViewSizeX, GS.HeroPos.y - ViewSizeY, GS.HeroPos.y + ViewSizeY);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glLoadIdentity(); // reset the modelview

	glEnable (GL_TEXTURE_2D);
	Drawable::draw();

	//Draw the HUD over everything else
	hud->draw(static_cast<int>(GS.WindowSize.x), GS);

	glViewport(w - w/4, 0, w / 4, w / 4); // set viewport (drawing area) to entire window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // reset the projection
	gluOrtho2D(0, 512, 0, 512);
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
		if(key == 27)
			GS.KeyStates |= ESC;
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
		if(key == 27)
			exit(0);
	}

	void Scene::pressSpecialKey(int key, int xx, int yy){
	}
	void Scene::releaseSpecialKey(int key, int xx, int yy){
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
		GS.MousePos = Vector2d(xx,yy);
	}
	
	void Scene::Display()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // clear buffers
		glLoadIdentity(); // reset modelview transformation
		ptrInstance->draw(); // draw the world
		glutSwapBuffers(); // make it all visible
	}

	void Scene::Reshape(int w, int h)
	{
		static bool FirstTime = true;
		if(FirstTime)
		{
			FirstTime = false;
			glShadeModel(GL_SMOOTH);
			
			
			//Load the Textures with the texture_manager
			texture_manager::load_texture("Level1.bmp", 512, 512);
			texture_manager::load_texture("Spider.bmp", 256, 256);
			texture_manager::load_texture("Flame.bmp", 128, 128);
			texture_manager::load_texture("HUD.bmp", 1024, 256);

			texture_manager::load_texture("Arrow.bmp", 64, 64);

			texture_manager::load_texture("Pistol.bmp", 64, 64);
			texture_manager::load_texture("MachineGun.bmp", 64, 64);
			texture_manager::load_texture("Shotgun.bmp", 64, 64);
			texture_manager::load_texture("AutoShotgun.bmp", 64, 64);
			texture_manager::load_texture("Flamethrower.bmp", 64, 64);
			texture_manager::load_texture("GrenadeLauncher.bmp", 64, 64);
			texture_manager::load_texture("BFG.bmp", 64, 64);
			texture_manager::load_texture("FireBombGun.bmp", 64, 64);
			
			glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			//Load the material (everything uses the same material)
			const GLfloat MatColor[] = {1, 1, 1, 1.0};
			const GLfloat Shininess[] = {10};

			//Allow transparency in textures
			glEnable (GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, MatColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, MatColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, Shininess );

			//Init everything after textures and whatnot are setup
			ptrInstance->InitGame();
		}

		GS.WindowSize = Vector2d(w,h);
	}

	void Scene::Idle()
	{
		
	}