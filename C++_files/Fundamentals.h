#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include "Object.h"
#include "Camera.h"
#include "VectorUtils3.h"

class Fundamentals
{
public:
    Fundamentals(Camera* cam);
    void loadfiles();
    void cameraCollision();
    void update();


private:

  Model *bookModel, *bookbackModel, *skybox, *carModel;
  mat4 projectionMatrix, camMatrix;
  vec3 p,l, c, cam, lookAtPoint, c2;
  GLuint program, skyboxProg, pageShader;
  GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 init = {0.0f, 0.0f, 0.0f};
  vec3 backPos = {-15.17f, 2.5f, 0.0f};
  vec3 topPos = {0.0f, 5.3f, 0.0f};
  bool cameraCollisionFlag = false;
  Camera* camera;
  Object* car;
  Object* bookback;
  Object* toppage;
  Object* bottompage;
  Object* pageStraight;
  Object* pageBent;

};

#endif
