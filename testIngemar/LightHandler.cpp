//#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LightHandler.h"
//#include "LightSource.h"

LightHandler::LightHandler()
{
  amount_of_lights = 0;
  positionArray = (vec3*)malloc(amount_of_lights*sizeof(vec3));
  ambientArray = (vec3*)malloc(amount_of_lights*sizeof(vec3));
  specularArray = (vec3*)malloc(amount_of_lights*sizeof(vec3));
  diffuseArray = (vec3*)malloc(amount_of_lights*sizeof(vec3));
  colourArray = (vec3*)malloc(amount_of_lights*sizeof(vec3));
  constantArray = (int*)malloc(amount_of_lights*sizeof(int));
  quadraticArray = (int*)malloc(amount_of_lights*sizeof(int));
  linearArray = (int*)malloc(amount_of_lights*sizeof(int));
}

int LightHandler::addLight()
{
  amount_of_lights ++;
  int index = amount_of_lights;
  //vec3 lightColour = light->getColour();
  vec3 lightColour = {1.0f, 1.0f, 1.0f};
  colourArray = (vec3*)realloc(colourArray, amount_of_lights*sizeof(vec3));
  colourArray[index] = lightColour;
  return index;
}

vec3 LightHandler::getColourArray()
{
  return *colourArray;
}
