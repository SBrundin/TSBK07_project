#ifndef CAMERA_H
#define CAMERA_H

#include "VectorUtils3.h"
#include "Object.h"

class Camera
{
public:
  void initate();
  void keyboardInput();
  void getMouse(int x, int y);
  bool CheckCollision(Object* two);
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
  vec3 size = vec3(1.0f, 1.0f, 1.0f);
  vec3 position, oldPosition, up, lookAtPoint, c;
  mat4 proj_matrix, camMatrix;

};

#endif
