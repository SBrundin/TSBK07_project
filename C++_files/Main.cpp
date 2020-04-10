
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
#include "Fundamentals.h"
#include "Camera.h"
#include "AndreasClass.h"
#include "Lamp.h"
#include "LightSource.h"


Fundamentals* loader;
Camera* camera;

void mouseControl(int x, int y)
{
	camera->getMouse(x,y);
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void init(void)
{
	camera = new Camera();
	camera->initate();
	loader = new Fundamentals(camera);
	loader->loadfiles();
	//loader->initiate();
}

void display(void)
{
	glutPassiveMotionFunc(mouseControl);
	camera->keyboardInput();
	loader->update();
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
	init ();
	glutDisplayFunc(display);

	glutTimerFunc(20, &timer, 0);
	glutMainLoop();
	exit(0);
}
