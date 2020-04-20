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

  Model *bottomModel, *backModel, *topModel, *straightPageModel, *bentPageModel, *carModel, *coronaModel1, *truckModel, *coronaModel2, *lampModel, *boxModel;
  Model *skybox[6];
  Lamp* lampLight;
  LightSource *lightSource, *dirrLight, *spotLight;
  LightHandler *lightHandler;
  mat4 projectionMatrix, camMatrix;
<<<<<<< HEAD
  vec3 p,l, c, cam, lookAtPoint, c2;
  GLuint program, skyboxProg, pageShader, programObj, fadeShade;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, bilTex, paperTex , truckTex;;
=======
  vec3 p,l, c, cam, lookAtPoint, c2, lampColour, lightPos, lightColour;
  GLuint program, skyboxProg, pageShader, programObj, lampProg, mainProg;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, bilTex, paperTex , truckTex;
>>>>>>> 079b9949a766ff10a236525868b878d0c28dd868
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 nullVec = {0.0f, 0.0f, 0.0f};
  vec3 backPos = {-15.17f, 2.5f, 0.0f};
  vec3 bentPos = {0.0f, 5.0f, 0.0f};
  vec3 straightPos = {0.0f, 4.3f, 0.0f};
  vec3 topPos = {0.0f, 5.3f, 0.0f};
  vec3 rotationAxis = {-15.0f, 0.0f, 0.0f};
  vec3 p1 = {-15.0f, 5.3f, -10.0f};
  vec3 p2 = {-15.0f, 5.3f, 10.0f};
  bool cameraCollisionFlag = false;
  Camera* camera;
  Object* car;
  Object* bookback;
  Object* toppage;
  Object* bottompage;
  Object* pageStraight;
  Object* pageBent;
  Object* coronaSimple;
  Object* coronaBase;
  Object* truck;
  Book* book;
  TextureData skytex[6*2];
  Object* lamp;
  Object* box;

};

#endif
