#ifndef CAMERA_H
#define CAMERA_H

#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "Object.h"

class Camera
{
public:
  void initate();
  void keyboardInput();
  void getMouse(int x, int y);
  bool CheckCollision(Object* two, bool flag);
  void checkFlag(bool flag);

  vec3 getPosition() const
  {
    return position;
  }
  mat4 getProj_matrix() const
  {
    return proj_matrix;
  }
  mat4 getCamMatrix() const
  {
    return  lookAt(position.x, position.y, position.z, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, up.x, up.y, up.z);
  }

private:
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 position, oldPosition, up, lookAtPoint, c;
  mat4 proj_matrix, camMatrix;
  GLfloat totX = 0;
  GLfloat totY = 500;
  GLfloat mspeed = 0.1;
  GLfloat midX = glutGet(GLUT_WINDOW_WIDTH)/2;
  GLfloat	midY = glutGet(GLUT_WINDOW_HEIGHT)/2;

};

#endif
