#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H
#include <stdio.h>
#include <list>
#include <vector>
#include "VectorUtils3.h"
#include "LightSource.h"


class LightHandler
{
public:
    LightHandler();
  //  int addLight(vec3 position, vec3 col, GLfloat constant, GLfloat linear, GLfloat quadratic); //Adds light and returns index of that light;
    void uploadPointLights(GLuint shader);
    void setPosition(int index, vec3 pos);
    //void changeLightPos(int index, vec3 pos);
    //vec3 getColourArray();
    //void changeLightColour(int index, vec3 col);
    //void sendLightsToShader();



private:
   std::vector<LightSource> lightVec;
    GLuint amount_of_lights;
};

#endif
