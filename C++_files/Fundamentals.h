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
    void loadtextures();
    void initshaders();
    void loadmodels();
    void cameraCollision();
    void update();
    void loadskybox();
    void initobjects();
    void drawall();
    mat4 rotate(Object* obj, GLfloat time);
    void fadeOutObjects();
    void fadeInObjects();

    GLfloat getMyTimer(){
      return _myTime;
    }

    void setMyTimer(GLfloat t){
      _myTime = t;
    }

    void increaseMyTimer(GLfloat t){
      _myTime += t;
    }

    void decreaseMyTimer(GLfloat t){
      _myTime -= t;
    }



private:

  Model *topModel, *frameModel, *pagesModel, *firstModel, *secondModel, *carModel, *coronaModel1, *truckModel, *coronaModel2, *lampModel, *boxModel;
  Model *skybox[6], *bookMarkModel;
  Lamp* lampLight;
  LightSource *lightSource, *dirrLight, *spotLight;
    LightHandler *pointLightVec;
  mat4 projectionMatrix, camMatrix, camMat2;
  vec3 p,l, c, cam, lookAtPoint, c2, lampColour, lightPos, lightColour;
  GLuint program, skyboxProg, pageShader, programObj, lampProg, mainProg, fadeShade;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, bilTex, paperTex, truckTex;
  GLuint woodTex, wood2Tex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  int pointLightIndex;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 initOrigin = {0.0f, 0.0f, 0.0f};
  vec3 initFirst = {0.21f, 3.133f, 0.0f};
  vec3 initSecond = {0.06f, 2.78f, 0.0f};
  vec3 initTop = {0.435f, 3.625f, 0.0f};
  vec3 rotationAxis = {-14.565f, 0.0f, 0.0f};
  bool cameraCollisionFlag = false;
  GLfloat t;
  Camera* camera;
  Object* car;
  Object* toppage;
  Object* pages;
  Object* frame;
  Object* firstPage;
  Object* secondPage;
  Object* bookMark;
  TextureData skytex[6*3];

  //page 1
  Object* coronaSimple;
  Object* coronaBase;
  Object* truck;
  Book* book;
  Object* lamp;
  Object* box;
  GLfloat _myTime;

  //page 2
  Object* house;
  Object* cottage;
  Object* cottage1;
  Object* cottage2;
  Object* elephant;
  Object* elephantbby;
  Object* tree;
  Object* rosebush;
  Object* pile;

  Model *houseModel, *cottageModel, *elephantModel, *treeModel, *rosebushModel, *pileModel;

  //page 3
  Object* velociraptor1;
  Object* velociraptor2;
  Object* velociraptor3;
  Object* velociraptor4;
  Object* velociraptor5;
  Object* stegos;
  Object* trex;

  Model *velociModel, *stegosModel, *trexModel;
};

#endif
