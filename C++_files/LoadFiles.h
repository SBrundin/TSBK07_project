#ifndef LOADFILES_H
#define LOADFILES_H

#include "VectorUtils3.h"

class LoadFiles
{
public:
    void initiate(mat4 projectionMatrix);
    void update(mat4 projectionMatrix, mat4 camMatrix);
    void keyboardInput();
    void getMouse(int x, int y);

private:

  Model *boktop, *bokrygg, *skybox;
  mat4 rot, trans, rot2, trans2, total2, totalBook, totalBook2, projectionMatrix2;
  vec3 p,l, c, cam, lookAtPoint, c2;
  GLuint program, skyboxProg, pageShader;
  GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
  GLfloat viewX = 0.5;
  GLfloat viewY = 0.5;
  vec3 v = {0.0, 1.0, 0.0};
  GLfloat Bookx = 0.0f;
  GLfloat Bookz = 0.0f;
  GLfloat Booky = 0.0f;
};

#endif
