#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include "Object.h"
#include "Book.h"
#include "Camera.h"
#include "VectorUtils3.h"

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

  Model *bottomModel, *backModel, *topModel, *straightPageModel, *bentPageModel, *carModel, *coronaModel1, *truckModel, *coronaModel2;
  Model *skybox[6];
  mat4 projectionMatrix, camMatrix;
  vec3 p,l, c, cam, lookAtPoint, c2;
  GLuint program, skyboxProg, pageShader, programObj;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, bilTex, paperTex , truckTex;;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 nullVec = {0.0f, 0.0f, 0.0f};
  vec3 backPos = {-15.17f, 2.5f, 0.0f};
  vec3 bentPos = {0.0f, 5.0f, 0.0f};
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

};

#endif
