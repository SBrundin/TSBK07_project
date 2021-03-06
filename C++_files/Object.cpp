#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <iostream>
#include <array>
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

Object::Object(vec3 pos, Model* model, GLuint tex, GLuint texSide, GLuint texUp)
{
    _position = pos;
    _model = model;
    _texture = tex;
    _size = vec3(0.0f, 0.0f, 0.0f);
    _textureSide = texSide;
    _textureUp = texUp;
    setBoundingBox();
}

Object::Object(Model* model, GLuint tex, GLuint texSide, GLuint texUp)
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _model = model;
    _texture = tex;
    _size = vec3(0.0f, 0.0f, 0.0f);
    _textureSide = texSide;
    _textureUp = texUp;

    setBoundingBox();
}

Object::Object(Model* model, GLuint tex, GLuint texSide, GLuint texUp0, GLuint texUp1, GLuint texUp2)
{
    _position = vec3(0.0f, 0.0f, 0.0f);
    _model = model;
    _texture = tex;
    _size = vec3(0.0f, 0.0f, 0.0f);
    _textureSide = texSide;
    _textureUp0 = texUp0;
    _textureUp1 = texUp1;
    _textureUp2 = texUp2;
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

vec3 Object::getTextureUp()
{
    return vec3(_textureUp0, _textureUp1, _textureUp2);
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




void Object::drawOn(mat4 camMatrix, GLuint shader, float scale, mat4 rot, Object* object){

  GLfloat xx = _position.x - object->getPosition().x;
  GLfloat zz = _position.z - object->getPosition().z;
  if(_position.x < -15.0){
    _position.y = -object->getRealHeight(-xx , zz, object, 1) + object->getPosition().y +  _size.y/2;
  }
  else{
    _position.y = object->getRealHeight(xx , zz, object, 0) + object->getPosition().y +  _size.y/2;

  }
  draw(camMatrix, shader, scale, rot);
}

void Object::draw(mat4 camMatrix, GLuint shader, float scale, mat4 rot)
 {
   vec4 opac = vec4(-100.0f,-100.0f,-100.0f,-100.0f);
   mat4 modelView = T(_position.x, _position.y, _position.z);
   modelView = Mult(modelView, Mult(S(scale,scale,scale), rot));
   mat4 Tot = Mult(camMatrix, modelView);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, getTexture());
   glUniform1i(glGetUniformLocation(shader, "Tex"), 0); // Texture unit 0
   glUniform4fv(glGetUniformLocation(shader, "opac"), 1, &opac.x);
   glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, Tot.m);
   glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_TRUE, modelView.m);
   DrawModel(getModel(), shader, "inPosition", "inNormal", "inTexCoord");
 }

 void Object::drawOver(mat4 camMatrix, GLuint shader, float scale, mat4 rot, vec4 opac)
  {
    mat4 modelView = T(_position.x, _position.y, _position.z);
    modelView = Mult(modelView, Mult(S(scale,scale,scale), rot));
    mat4 Tot = Mult(camMatrix, modelView);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTexture());
    glUniform1i(glGetUniformLocation(shader, "Tex"), 0); // Texture unit 0
    glUniform4fv(glGetUniformLocation(shader, "opac"), 1, &opac.x);
    glUniformMatrix4fv(glGetUniformLocation(shader, "myRotZ"), 1, GL_TRUE, rot.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, Tot.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_TRUE, modelView.m);
    DrawModel(getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  }

GLfloat Object::getCorrHeightInt(int x, int z){
	return _model->vertexArray[(x + z)*3 + 1];
}

GLfloat Object::getRealHeight(GLfloat x, GLfloat z, Object* object, GLint flipped){
    Model* placeon = object->getModel();
    GLfloat xx, zz, tempdist, height;
    GLfloat dist = 1000000;

    for (int i = 0; i < placeon->numVertices; i++){
      xx = placeon->vertexArray[3 * i];
      zz = placeon->vertexArray[3 * i + 2];
      tempdist = sqrt(pow((xx - x), 2) + pow((zz - z), 2));
      if (tempdist < dist){
          height = placeon->vertexArray[3 * i + 1];
          dist = tempdist;
        }
        if (flipped == 0){
          if (tempdist == dist && height < placeon->vertexArray[3 * i + 1]){
                  height = placeon->vertexArray[3 * i + 1];
                  dist = tempdist;
                }
              }
        if (flipped == 1){
          if (tempdist == dist && height > placeon->vertexArray[3 * i + 1]){
                  height = placeon->vertexArray[3 * i + 1];
                  dist = tempdist;
                }
              }
      }
      return height;
    }

void Object::rotate(mat4 rotation)
    {

      vec4 size4 = vec4(_position.x, _position.y, _position.z, 1);
      vec4 temp =  MultVec4(rotation, size4);
      _position.x=(abs(temp.x / temp.w));
      _position.y=(abs(temp.y / temp.w));
      _position.z=(abs(temp.z / temp.w));
      //_size = ScalarMult(_size, scale);
      //std::cout << _size.x << ' ' << _size.y << ' ' << _size.z << '\n';
    }
