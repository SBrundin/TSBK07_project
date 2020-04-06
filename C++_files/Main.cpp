
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
#include "LoadFiles.h"
#include "AndreasClass.h"


//AndreasClass a;
LoadFiles* loader;
void mouseControl(int x, int y)
{

	loader->getMouse(x,y);
	glutPostRedisplay();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void init(void)
{
	loader = new LoadFiles();
	loader->initiate();
}

void display(void)
{

	loader->update();
	loader->keyboardInput();


 	printError("display");
 	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("GLUTen project");
	glutDisplayFunc(display);
#ifdef WIN32
	glewInit();
#endif
	init ();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouseControl);
	glutMainLoop();
	exit(0);
}
