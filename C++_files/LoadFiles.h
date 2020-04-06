#ifndef LOADFILES_H
#define LOADFILES_H

#include "Objects.h"
#include "Camera.h"
#include "VectorUtils3.h"

class LoadFiles
{
public:
    LoadFiles(Camera* camera);
    void initiate(mat4 projectionMatrix);
    //void update(Camera* camera);
    void update(Camera* camera);
    void keyboardInput();
    void getMouse(int x, int y);
    //Objects* getObject();

private:

  Model *boktop, *bokrygg, *skybox, *car;
  mat4 rot, trans, rot2, trans2, total2, totalBook, totalBook2, projectionMatrix, camMatrix;
  vec3 p,l, c, cam, lookAtPoint, c2;
  GLuint program, skyboxProg, pageShader;
  GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  GLfloat Bookx = 0.0f;
  GLfloat Bookz = 0.0f;
  GLfloat Booky = 0.0f;
  Camera* camera;
  //Objects* _objects;

};

#endif
