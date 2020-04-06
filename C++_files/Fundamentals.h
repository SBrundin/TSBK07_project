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
    void initiate(mat4 projectionMatrix);
    void update();
    void keyboardInput();
    void getMouse(int x, int y);

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
  vec3 upperCoord = {0.0f, 2.5f, 0.0f};
  Camera* camera;
  Object* car;
  Object* bookback;
  Object* toppage;
  Object* bottompage;

};

#endif
