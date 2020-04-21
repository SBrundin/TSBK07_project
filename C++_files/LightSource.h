#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <stdio.h>
#include "VectorUtils3.h"
#include "Lamp.h"

class LightSource
{
public:
    LightSource(vec3 position, vec3 colour, float constant, float linear, float quadratic);


    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getViewPosition();
    void setViewPosition(vec3 position);

    vec3 getColour();
    void setColour(vec3 col);

    vec3 getAmbient();
    void setAmbient(vec3 amb);

    vec3 getDiffuse();
    void setDiffuse(vec3 diff);

    vec3 getSpecular();
    void setSpecular(vec3 spec);

    vec3 getViewPos();
    void setViewPos(vec3 viewPos);

    GLfloat getConstant();
    void setConstant(float con);

    GLfloat getLinear();
    void setLinear(float lin);

    GLfloat getQuadratic();
    void setQuadratic(float qua);

    vec3 getDirection();
    void setDirection(vec3 dir);

    GLfloat getCutOff();
    void setCutOff(GLfloat co);

    GLfloat getOuterCutOff();
    void setOuterCutOff(GLfloat oCo);
    void uploadDirLight(GLuint shader);
    void updateDirection(GLuint shader, vec3 dir);





private:
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPos;
    vec3 colour;
    vec3 direction;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
    GLfloat cutOff;
    GLfloat outerCutOff;
};

#endif
