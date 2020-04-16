#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LightSource.h"

LightSource::LightSource(vec3 position, vec3 col, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
    ambient = {0.2f, 0.2f, 0.2f};//This is also where you set the colour
    diffuse = {0.5f, 0.5f, 0.5f}; //This is where you set the colour
    specular = {1.0f, 1.0f, 1.0f};
    direction = {-0.2f, -1.0f, -0.3f};
    colour = col;
    specular = specular;
    constant = constant;
    linear = linear;
    quadratic = quadratic;
}



vec3 LightSource::getPosition()
{
    return position;
}

void LightSource::setPosition(vec3 pos)
{
    position = pos;
}

vec3 LightSource::getColour()
{
  return colour;
}

void LightSource::setColour(vec3 col)
{
  colour = col;
}


vec3 LightSource::getAmbient()
{
  return ambient;
}

void LightSource::setAmbient(vec3 amb)
{
  ambient = amb;
}

vec3 LightSource::getDiffuse()
{
  return diffuse;
}

void LightSource::setDiffuse(vec3 diff)
{
  diffuse = diff;
}

vec3 LightSource::getSpecular()
{
  return specular;
}

void LightSource::setSpecular(vec3 spec)
{
  specular = spec;
}

GLfloat LightSource::getConstant()
{
  return constant;
}

void LightSource::setConstant(float constant)
{
  constant = constant;
}

GLfloat LightSource::getLinear()
{
  return linear;
}

void LightSource::setLinear(float lin)
{
  linear = lin;
}

void LightSource::setQuadratic(float qua)
{
  quadratic = qua;
}

GLfloat LightSource::getQuadratic()
{
  return quadratic;
}



void LightSource::setViewPos(vec3 pos)
{
  viewPos = pos;
}


vec3 LightSource::getViewPos()
{
  return viewPos;
}

void LightSource::setDirection(vec3 dir)
{
  direction = dir;
}


vec3 LightSource::getDirection()
{
  return direction;
}
