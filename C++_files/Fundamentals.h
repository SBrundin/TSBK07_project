#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include "Object.h"
#include "Camera.h"
#include "VectorUtils3.h"
#include "Lamp.h"
#include "LightSource.h"
#include "LightHandler.h"

class Fundamentals
{
public:
    Fundamentals(Camera* cam);
    void loadfiles();
    void cameraCollision();
    void update();


private:

  Model *bookModel, *bookbackModel, *skybox, *carModel, *boxModel;
  Lamp* lampLight;
  LightSource *lightSource, *dirrLight, *spotLight;
  LightHandler *lightHandler;
  mat4 projectionMatrix, camMatrix;
  vec3 p,l, c, cam, lookAtPoint, c2, lampColour, lightPos, lightColour;
  GLuint program, skyboxProg, pageShader, lampProg, mainProg;
  GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 init = {0.0f, 0.0f, 0.0f};
  vec3 upperCoord = {0.0f, 2.5f, 0.0f};
  bool cameraCollisionFlag = false;
  Camera* camera;
  Object* car;
  Object* bookback;
  Object* toppage;
  Object* bottompage;
  Object* lamp;
  Object* box;
};

#endif
