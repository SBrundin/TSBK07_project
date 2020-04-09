#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "Lamp.h"

Lamp::Lamp(Object* obj)
{
    object = obj;
    position = object->getPosition();
    colour = {1.0f, 0.4f, 0.1f};
}



vec3 Lamp::getPosition()
{
    return object -> getPosition();
}

void Lamp::setPosition(vec3 position)
{
    position = position;
}

Object* Lamp::getObject()
{
    return object;
}

void Lamp::setObject(Object* obj)
{
    object = obj;
}

vec3 Lamp::getColour()
{
  return colour;
}

void Lamp::setColour(vec3 col)
{
  colour = col;
}
