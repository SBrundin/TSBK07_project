#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include "Object.h"
#include "Book.h"
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
    void loadskybox();
    mat4 rotate(Object* obj, GLfloat time);


private:

  Model *topModel, *frameModel, *pagesModel, *firstModel, *secondModel, *carModel, *coronaModel1, *truckModel, *coronaModel2, *lampModel, *boxModel;
  Model *skybox[6];
  Lamp* lampLight;
  LightSource *lightSource, *dirrLight, *spotLight;
  LightHandler *lightHandler;
  mat4 projectionMatrix, camMatrix;
  vec3 p,l, c, cam, lookAtPoint, c2, lampColour, lightPos, lightColour;
  GLuint program, skyboxProg, pageShader, programObj, lampProg, mainProg, fadeShade;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, bilTex, paperTex , truckTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 initOrigin = {0.0f, 0.0f, 0.0f};
  vec3 initFirst = {0.0f, 3.0f, 0.0f};
  vec3 initSecond = {0.0f, 2.6f, 0.0f};
  vec3 initTop = {0.4f, 3.6f, 0.0f};
  vec3 rotationAxis = {-15.0f, 0.0f, 0.0f};
  bool cameraCollisionFlag = false;
  Camera* camera;
  Object* car;
  Object* toppage;
  Object* pages;
  Object* frame;
  Object* firstPage;
  Object* secondPage;
  Object* coronaSimple;
  Object* coronaBase;
  Object* truck;
  Book* book;
  TextureData skytex[6*2];
  Object* lamp;
  Object* box;


};

#endif
