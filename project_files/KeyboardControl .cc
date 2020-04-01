
#include "KeyboardControl.h"
#include "MicroGlut.h"


KeyboardControl::moveMouse(int mouseX, int mouseY, int height, int width)
{
  	viewX = (float)x/glutGet(width)*2*M_PI;
  	viewY = (float)y/glutGet(height)*M_PI;

  	lookAtPoint.x = -10 *sin(viewY)*sin(viewX) + cam.x;
  	lookAtPoint.y = 10*cos(viewY) + cam.y;
  	lookAtPoint.z = 10 *sin(viewY)*cos(viewX) + cam.z;

  	glutPostRedisplay();

};
