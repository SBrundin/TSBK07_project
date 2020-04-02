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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main(int argc, char* argv[])
{

  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("GLUTen Project");
  glEnable(GL_CULL_FACE);
  glutMainLoop();
  return 0;
}
