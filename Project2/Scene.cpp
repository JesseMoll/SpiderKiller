#include "Scene.h"
#include "Hero.h"
#include "CreepSpawner.h"
#include "texture_manager.h"

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
}

void Scene::InitGame()
{
	SetupWallRepulsionArray();
	new (&GS) GlobalState();
    GS.TheHero = AddChild(new Hero(this, 0, Vector2d(TextureWidth/2, TextureWidth/2)));	
	AddChild(new CreepSpawner(this));
	
}


void Scene::draw()
{


    glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	const int ViewSize = 60;

	gluOrtho2D(GS.HeroPos.x - ViewSize, GS.HeroPos.x + ViewSize, GS.HeroPos.y - ViewSize, GS.HeroPos.y + ViewSize);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode


	glLoadIdentity(); // reset the modelview
	
	//Turn Textures on for the ground
	glEnable (GL_TEXTURE_2D);
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
	//Turn off textures when drawing everything else
	glDisable(GL_TEXTURE_2D);

	//Draw the children... 
	Drawable::draw();
	//Reenable the mouse callback (we only check the mouse at most once every frame)
	GS.MouseSemaphore = false;
}

void Scene::Timer(int value){

    std::chrono::time_point<std::chrono::system_clock> CurrentTime = std::chrono::system_clock::now();
	__int64 msSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastUpdate).count();
	LastUpdate = CurrentTime;

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
			exit(0);
		if(key == 27)
			exit(0);
	}

	void Scene::pressSpecialKey(int key, int xx, int yy){
	}
	void Scene::releaseSpecialKey(int key, int xx, int yy){
	}

	void Scene::mouseMove(int xx, int yy){
		//check if the scene has been redrawn since the last check
		//Otherwise it may have been called because of the warp pointer call
		if(GS.MouseSemaphore)
			return;
		GS.MouseSemaphore = true;
		//TODO Do Mouse Dragging stuff
		//Probably wont need to do anything here
		//this will capture the pointer to allow unlimited dragging (for rotation and whatnot)
		//glutWarpPointer((int)GS.MousePos.x, (int)GS.MousePos.y);
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
		if(w > h)
		{
			glViewport((w-h)/2, 0, h, h); // set viewport (drawing area) to entire window
		}
		else
		{
			glViewport(0, (h-w)/2, w, w); // set viewport (drawing area) to entire window
		}
	}

	void Scene::Idle()
	{
		
	}