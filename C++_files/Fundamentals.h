#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include "Object.h"
#include "Book.h"
#include "Camera.h"
#include "VectorUtils3.h"
#include "Lamp.h"
#include "LightSource.h"
#include "LightHandler.h"
#include <vector>
#include <iostream>

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
    mat4 rotate(Object* obj, GLfloat time);
    void fadeOutObjects();
    void fadeInObjects();
    void drawFirstScene();
    void drawSecondScene();
    void drawSkybox();
    void drawLights();
    void initLights();
    void drawPointLight(int index, LightSource* light, GLuint shader);
    void drawDirLight(int index, LightSource* light, GLuint shader);
    void drawSpotLight(int index, LightSource* light, GLuint shader);
    void drawLightsScene1(GLuint objShader);
    void drawLightsScene2(GLuint objShader);
    void drawLightsScene0(GLuint objShader);


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

    void updateRotangle(GLfloat t){
      _rotangle += t;
    }

private:
  std::vector<Object*> listOfObj_1;
  std::vector<Object*> listOfObj_2;
  std::vector<Object*> listOfObj_3;
  Model *topModel, *frameModel, *pagesModel, *firstModel, *secondModel, *carModel, *coronaModel1, *truckModel, *coronaModel2, *lampModel, *boxModel;
  Model *skybox[6], *bookMarkModel, *sunModel, *moonModel, *mountainModel, *cloudModel, *rainbowModel, *backgroundModel, *stopModel, *trafficModel, *streetLightModel;
  Model *manModel, *trashcanModel;
  Lamp* lampLight;
  LightSource *lightSource, *dirrLight, *spotLight, *pointLight0, *spotLight0, *dirLight0, *sunLight1, *spotLight1, *spotLight2, *dirLight1, *streetLight1, *dirrLight2;
  LightSource *bookSpot1, *bookSpot2,*bookSpot3, *bookSpot4, *bookDir, *moonLight;
  LightHandler *pointLightVec;
  mat4 projectionMatrix, camMatrix, camMat2, backgroundrot;
  vec3 p,l, c, cam, lookAtPoint, c2, lampColour, lightPos, lightColour, pointLightPos0, pointLightColour0;
  vec3 spotLightColour0, spotLightPos0, dirLightPos0, dirLightColor0;
  vec3 viewPos, sunColour, sunPosition;
  GLuint program, skyboxProg, pageShader, programObj, lampProg, mainProg, fadeShade;
  GLuint sphereTex, waterTex, snowTex, grassTex, tex2, bookTex, leatherTex, leather2Tex, bilTex, paperTex, truckTex, asphaltTex, backgroundTex, bricksTex, bricks2Tex;
  GLuint woodTex, wood2Tex, cloudTex, crackedmudTex, crackedmud2Tex, crackedmud3Tex, pathTex, greenTex, moonTex, sunTex, stoneTex, rainbowTex;
  GLuint grass1Tex, grass2Tex, grass3Tex, grass4Tex, grass5Tex, grass6Tex, cottageTex, sandTex, sand1Tex, sand2Tex, lavaTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  GLfloat dir = 0.01;
  GLfloat _rotangle;
  GLfloat _lane;
  GLfloat constant, linear, quadratic;
  int pointLightIndex;
  vec3 v = {0.0, 1.0, 0.0};
  vec3 rotationAxis = {-14.565f, 0.0f, 0.0f};
  bool cameraCollisionFlag = false;
  GLfloat t;
  GLfloat oldt = 0;
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
  GLfloat _myTime = 0.0f;

  //page 2
  Object* house;
  Object* cottage;
  Object* cottage1;
  Object* cottage2;
  Object* elephant;
  Object* elephantbby;
  Object* tree;
  Object* tree2;
  Object* rosebush1;
  Object* rosebush2;
  Object* rosebush3;
  Object* pile;
  Object* bird;
  Object* bird2;
  Object* bird3;
  Object* background;
  Object* sun;
  Object* moon;
  Object* mountain;
  Object* mountain2;
  Object* cloud;
  Object* rainbow;

  Model *houseModel, *cottageModel, *elephantModel, *treeModel, *rosebushModel, *pileModel, *birdModel;

  //page 3
  Object* velociraptor1;
  Object* velociraptor2;
  Object* velociraptor3;
  Object* velociraptor4;
  Object* velociraptor5;
  Object* velociraptor6;
  Object* velociraptor7;
  Object* stegos1;
  Object* stegos2;
  Object* stegos3;
  Object* trex;
  Object* stopSign;
  Object* streetLight;
  Object* trafficLight;
  Object* man;
  Object* trashcan;
  Object* fence;

  Model *velociModel, *stegosModel, *trexModel, *fenceModel;
};

#endif
