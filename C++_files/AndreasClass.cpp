#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "AndreasClass.h"
#include <iostream>
using namespace std;

AndreasClass::AndreasClass(){
  cout << "Andreas är född" << endl;
}
