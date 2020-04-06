#include "Camera.h"
#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "Object.h"


void Camera::initate(){
    position = SetVector(0, 5, 8);
    lookAtPoint = SetVector(0,5,0);
    up = SetVector(0,1,0);
    proj_matrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);
}

void Camera::getMouse(int x, int y){
  viewX = (float)x/600*2*M_PI;
  viewY = (float)y/600*M_PI;

  lookAtPoint.x = -10 *sin(viewY)*sin(viewX) + position.x;
  lookAtPoint.y = 10*cos(viewY) + position.y;
  lookAtPoint.z = 10 *sin(viewY)*cos(viewX) + position.z;

}

void Camera::keyboardInput(){
  	vec3 forward = VectorSub(position, lookAtPoint);
    oldPosition = position;

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

bool Camera::CheckCollision(Object* two) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = position.x + size.x >= two->getPosition().x && two->getPosition().x + two->getSize().x >= position.x;
	// Collision y-axis?
	bool collisionY = position.y + size.y >= two->getPosition().y && two->getPosition().y + two->getSize().y >= position.y;
	// Collision z-axis?
	bool collisionZ = position.z + size.z >= two->getPosition().z && two->getPosition().z + two->getSize().z >= position.z;
	// Collision only if on both axes
	return collisionX && collisionY && collisionZ;
}

void Camera::checkFlag(bool flag)
{
  if (flag){
    position = oldPosition;
  }
}
