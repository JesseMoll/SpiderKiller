//----------------------------------------------------------------------
//CSMC 425 Final Project
//----------------------------------------------------------------------
//Kill the spiders... Do it!
//Controls:
//		Q/Esc - Quit
//		WASD - Walk
//

// include files are in a slightly different location for MacOS
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "Scene.h" // world object definitions

//----------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------

Scene* theScene; // pointer to the scene (everything is in there, basically root of scene graph)


int main(int argc, char **argv)
{
	std::cout << "\n\
-----------------------------------------------------------------------\n\
  Kill the Spiders!:\n\
  - Hold WASD to move character around\n\
  - q, e and f to switch weapons \n\
  - Left mouse button for primary weapon \n\
  - Right mouse button for secondary weapon \n\
  - Space for super weapon \n\
  - Arrow keys to look around \n\
  - ~ to show debug information \n\
  - esc to quit\n\
-----------------------------------------------------------------------\n";



	glutInit(&argc, argv);
	theScene = Scene::Instance(); // Create the scene (Pretty much everything is contained within)
	glutMainLoop();

	return 0; // this is just to keep the compiler happy
}
