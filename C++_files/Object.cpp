#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include<iostream>
#include<array>
using namespace std;

Object::Object()
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _direction = vec3(0.0f, 0.0f, 0.0f);
    _size = vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(Model* model, GLuint tex)
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _model = model;
    _texture = tex;
    _size = vec3(0.0f, 0.0f, 0.0f);
    setBoundingBox();
}

Object::Object(vec3 pos, Model* model, GLuint tex)
{
    _position = pos;
    _model = model;
    _texture = tex;
    _size = vec3(0.0f, 0.0f, 0.0f);
    setBoundingBox();
}

vec3 Object::getPosition()
{
    return _position;
}

void Object::setPosition(vec3 position)
{
    _position = position;
}

vec3 Object::getSize()
{
  return _size;
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

GLuint Object::getTexture()
{
    return _texture;
}

GLuint Object::getTextureSide()
{
    return _textureSide;
}

GLuint Object::getTextureUp()
{
    return _textureUp;
}

void Object::setTexture(GLuint tex)
{
    _texture = tex;
}

void Object::setTextureSide(GLuint tex)
{
    _textureSide = tex;
}

void Object::setTextureUp(GLuint tex)
{
    _textureUp = tex;
}

void Object::setBoundingBox()
{
	int i;
	float maxx = -1e10, maxy = -1e10, maxz = -1e10, minx = 1e10, miny = 1e10, minz = 1e10;

	for (i=0; i < _model->numVertices; i++)
	{
		if (_model->vertexArray[3 * i] < minx) minx = _model->vertexArray[3 * i];
		if (_model->vertexArray[3 * i] > maxx) maxx = _model->vertexArray[3 * i];
		if (_model->vertexArray[3 * i+1] < miny) miny = _model->vertexArray[3 * i+1];
		if (_model->vertexArray[3 * i+1] > maxy) maxy = _model->vertexArray[3 * i+1];
		if (_model->vertexArray[3 * i+2] < minz) minz = _model->vertexArray[3 * i+2];
		if (_model->vertexArray[3 * i+2] > maxz) maxz = _model->vertexArray[3 * i+2];
	}
	_size = SetVector(maxx-minx, maxy-miny, maxz-minz);
  // fprintf(stderr, "maxx %f minx %f \n", maxx, minx);
  // fprintf(stderr, "maxy %f miny %f \n", maxy, miny);
  // fprintf(stderr, "maxz %f minz %f \n", maxz, minz);
  // std::cout <<  _model->numVertices << '\n';
}

void Object::updateBoundingBox(mat4 rotation, GLfloat scale)
{
  //std::cout << _size.x << ' ' << _size.y << ' ' << _size.z << '\n';

  vec4 size4 = vec4(_size.x*scale, _size.y*scale, _size.z*scale, 1);
  vec4 temp =  MultVec4(rotation, size4);

  _size.x = abs(temp.x / temp.w);
  _size.y = abs(temp.y / temp.w);
  _size.z = abs(temp.z / temp.w);
  //_size = ScalarMult(_size, scale);
  //std::cout << _size.x << ' ' << _size.y << ' ' << _size.z << '\n';
}
