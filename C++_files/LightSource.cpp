#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LightSource.h"

LightSource::LightSource(vec3 position, vec3 colour)
{
    position = position;
    colour = colour;
}



vec3 LightSource::getPosition()
{
    return position;
}

void LightSource::setPosition(vec3 position)
{
    position = position;
}


vec3 LightSource::getColour()
{
  return colour;
}

void LightSource::setColour(vec3 col)
{
  colour = col;
}

void LightSource::setViewPos(vec3 pos)
{
  viewPos = pos;
}


vec3 LightSource::getViewPos()
{
  return viewPos;
}
