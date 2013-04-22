#include "Scene.h"
#include "Hero.h"

const int TextureWidth = 512;
// Texture is a WxWx4 array (rgba)
static GLubyte TexImage[TextureWidth][TextureWidth][4];
//So OpenGL can access the texture
static GLuint TextureName;



// procedure to create a little texture by doing some random xoring and addition on the coords
void MakeTexture()
{
  for (int i = 0;i < TextureWidth;i++)
  { for (int j = 0; j < TextureWidth;j++)
    {
	if (GetWalkable((Vector2d(i,j))))
	{
      TexImage[i][j][0] = (GLubyte) 100;
      TexImage[i][j][1] = (GLubyte) 60;
      TexImage[i][j][2] = (GLubyte) 30;
      TexImage[i][j][3] = 255;
	}
	else
	{
      TexImage[i][j][0] = 25;
      TexImage[i][j][1] = 0;
      TexImage[i][j][2] = 0;
      TexImage[i][j][3] = 255;
	}

    }
  }
}


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
	glutInitWindowSize(400, 400);// Initial Window
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
    InitGame();
	glutTimerFunc(0, Timer, 0);

}

void Scene::DestructGame()
{
}

void Scene::InitGame()
{
	new (&GS) GlobalState();
    GS.TheHero = AddChild(new Hero(Vector2d(TextureWidth/2, TextureWidth/2), this, 0));	
}


void Scene::draw()
{


    glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	const int ViewSize = 40;

	gluOrtho2D(GS.CamPos.x - ViewSize, GS.CamPos.x + ViewSize, GS.CamPos.y - ViewSize, GS.CamPos.y + ViewSize);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode


	glLoadIdentity(); // reset the modelview
	
	
	//Turn Textures on for the ground
	glEnable (GL_TEXTURE_2D);

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
	long msSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastUpdate).count();
	LastUpdate = CurrentTime;

	if(ptrInstance->update(msSinceLastUpdate, GS) != UPDATE_NONE) // update everything and if anything has changed...
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
		glutWarpPointer(GS.MousePos.x, GS.MousePos.y);
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
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f); //Should never see this
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
	
			//Yeah, Textures have to be done elsewhere
			MakeTexture();
			glGenTextures (1,&TextureName);
			glBindTexture (GL_TEXTURE_2D,TextureName);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, TextureWidth, TextureWidth, GL_RGBA, GL_UNSIGNED_BYTE, TexImage);

			//Set up the material of the ground (this only needs to be done once too)
			glBindTexture (GL_TEXTURE_2D, TextureName);
			glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

			//Load the material (everything uses the same material)
			const GLfloat MatColor[] = {1, 1, 1, 1.0};
			const GLfloat Shininess[] = {10};

			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, MatColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, MatColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, Shininess );

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