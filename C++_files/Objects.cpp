#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"

Object::Object()
{
    _position = vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(Model* model)
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _model = model;
}

Object::Object(vec3 pos, Model* model)
{
    _position = pos;
    _model = model;
}

vec3 Object::getPosition()
{
    return _position;
}

void Object::setPosition(vec3 position)
{
    _position = position;
}

vec3 Object::getDirection()
{
    return _direction;
}

void Object::setDirection(vec3 direction)
{
    _direction = direction;
}

Model* Object::getModel()
{
    return _model;
}

void Object::setModel(Model* model)
{
    _model = model;
}
