#ifndef I_LOAD_TEX
#define I_LOAD_TEX

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#else
	#include <GL/gl.h>
#endif

GLuint LoadTexture(char const *filename, char dorepeat);

#endif
