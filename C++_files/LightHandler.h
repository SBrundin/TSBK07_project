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
    int addLight(vec3 position, vec3 col, GLfloat constant, GLfloat linear, GLfloat quadratic); //Adds light and returns index of that light;
    void uploadPointLights(GLuint shader);
    void setPosition(int index, vec3 pos);
    std::vector<vec3> getPosVec();
    std::vector<vec3> getColVec();
    std::vector<vec3> getAmbVec();
    std::vector<vec3> getDiffVec();
    std::vector<vec3> getSpecVec();
    std::vector<vec3> getDirVec();
    std::vector<GLfloat> getConstVec();
    std::vector<GLfloat> getLinVec();
    std::vector<GLfloat> getQuatVec();
    std::vector<GLfloat> getCutOffVec();
    std::vector<GLfloat> getOuterCutOffVec();
    //void changeLightPos(int index, vec3 pos);
    //vec3 getColourArray();
    //void changeLightColour(int index, vec3 col);
    //void sendLightsToShader();



private:
   std::vector<LightSource*> lightVec;
   std::vector<vec3> positionVec;
   std::vector<vec3> colourVec;
   std::vector<vec3> ambientVec;
   std::vector<vec3> diffuseVec;
   std::vector<vec3> specularVec;
   std::vector<GLfloat> constantVec;
   std::vector<GLfloat> linearVec;
   std::vector<GLfloat> quadraticVec;
   std::vector<vec3> directionVec;
   std::vector<GLfloat> cutOffVec;
   std::vector<GLfloat> outerCutOffVec;
    GLuint amount_of_lights;
};

#endif
