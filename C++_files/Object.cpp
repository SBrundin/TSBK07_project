#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"

Object::Object()
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _direction = vec3(0.0f, 0.0f, 0.0f);
    _size = vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(Model* model, vec3 pos, vec3 direction)
{
    _position = pos + direction * 0.5;
    _direction = direction * 2;
    _model = model;
    setBoundingBox();
}

vec3 Object::getPosition()
{
    return _position;
}

vec3 Object::getSize()
{
  return _size;
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

void Object::setBoundingBox()
{
	int i;
	float maxx = -1e10, maxy = -1e10, maxz = -1e10, minx = 1e10, miny = 1e10, minz = 1e10;

	for (i=0;i< _model->numVertices; i++)
	{
		if (_model->vertexArray[3 * i] < minx) minx = _model->vertexArray[3 * i];
		if (_model->vertexArray[3 * i] > maxx) maxx = _model->vertexArray[3 * i];
		if (_model->vertexArray[3 * i+1] < miny) miny = _model->vertexArray[3 * i+1];
		if (_model->vertexArray[3 * i+1] > maxy) maxy = _model->vertexArray[3 * i+1];
		if (_model->vertexArray[3 * i+2] < minz) minz = _model->vertexArray[3 * i+2];
		if (_model->vertexArray[3 * i+2] > maxz) maxz = _model->vertexArray[3 * i+2];
	}
	_size = SetVector(maxx-minx, maxx-minx, maxx-minx);
}
