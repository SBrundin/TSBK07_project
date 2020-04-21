#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LightSource.h"

LightSource::LightSource(vec3 position, vec3 col, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
    ambient = {0.2f, 0.2f, 0.2f};
    diffuse = {0.5f, 0.5f, 0.5f}; 
    specular = {1.0f, 1.0f, 1.0f};
    direction = {-0.2f, -1.0f, -0.3f};
    colour = col;
    specular = specular;
    constant = constant;
    linear = linear;
    quadratic = quadratic;
    cutOff = cos(  12.5f*2*M_PI/180  );
    outerCutOff = cos(  15.5f*2*M_PI/180 );
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

void LightSource::setConstant(GLfloat constant)
{
  constant = constant;
}

GLfloat LightSource::getLinear()
{
  return linear;
}

void LightSource::setLinear(GLfloat lin)
{
  linear = lin;
}

void LightSource::setQuadratic(GLfloat qua)
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

void LightSource::setCutOff(GLfloat co)
{
  cutOff = cos(  co*2*M_PI/180 );
}

GLfloat LightSource::getCutOff()
{
  return cutOff;
}

void LightSource::setOuterCutOff(GLfloat oCo)
{
  outerCutOff = cos(oCo*2*M_PI/180);
}

GLfloat LightSource::getOuterCutOff()
{
  return outerCutOff;
}

void LightSource::uploadDirLight(GLuint shader)
{
  glUniform3fv(glGetUniformLocation(shader, "dirLight.ambient"), 1, &ambient.x);
  glUniform3fv(glGetUniformLocation(shader, "dirLight.diffuse"), 1, &diffuse.x);
  glUniform3fv(glGetUniformLocation(shader, "dirLight.specular"), 1, &specular.x);
  glUniform3fv(glGetUniformLocation(shader, "dirLight.direction"), 1, &direction.x);
  glUniform3fv(glGetUniformLocation(shader, "dirLight.colour"), 1, &colour.x);
}

void LightSource::updateDirection(GLuint shader, vec3 dir)
{
  direction = dir;
  glUniform3fv(glGetUniformLocation(shader, "dirLight.direction"), 1, &direction.x);

}
