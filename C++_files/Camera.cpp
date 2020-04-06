#include "Camera.h"
#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <iostream>
using namespace std;



void Camera::initate(){
    position = SetVector(0, 5, 8);
    lookAtPoint = SetVector(2,0,2);
    up = SetVector(0,1,0);
    proj_matrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 5000.0);
}


void Camera::getMouse(int x, int y){
  if(x!=midX || y!=midY)
	{
    GLfloat deltaX = x - midX;
    GLfloat deltaY = y - midY;
    totX += deltaX; //+ totX;
    totY += deltaY; //+ totY;
    GLfloat angX = mspeed*(float)totX/glutGet(GLUT_WINDOW_WIDTH)*2*M_PI; //angle
    GLfloat angY = mspeed*(float)totY/glutGet(GLUT_WINDOW_HEIGHT)*2*M_PI;
    //angY = std::min((GLfloat)M_PI, angY);
    //angY = std::min((GLfloat)M_PI, angY);
    //printf("%g\n", lookAtPoint.z);
    lookAtPoint.x = -10*sin(angY)*sin(angX)+position.x;
    lookAtPoint.y = 10*cos(angY)+position.y;
    lookAtPoint.z = 10*sin(angY)*cos(angX)+position.z;
    glutWarpPointer(midX,midY);
    glutPostRedisplay();
  }


}

void Camera::keyboardInput(){
  	vec3 forward = VectorSub(position, lookAtPoint);

    if (glutKeyIsDown('w')) {
      c = Normalize(forward);
      c = ScalarMult(c,0.5);
      position = VectorSub(position, c);
      lookAtPoint = VectorSub(lookAtPoint, c);
    }
    if (glutKeyIsDown('s')) {
      c = Normalize(forward);
      c = ScalarMult(c,0.5);
      position = VectorAdd(position, c);
      lookAtPoint = VectorAdd(lookAtPoint, c);
    }
    if (glutKeyIsDown('a')) {
      c = CrossProduct(up, forward);
      c = Normalize(c);
      c = ScalarMult(c,0.5);
      position = VectorSub(position, c);
      lookAtPoint = VectorSub(lookAtPoint, c);
    }
    if (glutKeyIsDown('d')) {
      c = CrossProduct(up, forward);
      c = Normalize(c);
      c = ScalarMult(c,0.5);
      position = VectorAdd(position, c);
      lookAtPoint = VectorAdd(lookAtPoint, c);
    }
}
