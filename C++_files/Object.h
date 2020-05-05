#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include<iostream>
#include<array>


class Object
{
public:
    Object();
    Object(Model* model, GLuint tex);
    Object(vec3 pos, Model* model, GLuint tex);
    Object(Model* model, GLuint tex, GLuint texSide, GLuint texUp);
    Object(vec3 pos, Model* model, GLuint tex, GLuint texSide, GLuint texUp);
    Object(Model* model, GLuint tex, GLuint texSide, GLuint texUp0, GLuint texUp1, GLuint texUp2);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);
    void setY(GLfloat y){
      _position.y = y;
    }
    void setX(GLfloat x){
      _position.x = x;
    }

    void setZ(GLfloat z){
      _position.z = z;
    }
    Model* getModel();
    void setModel(Model* model);

    GLuint getTexture();
    GLuint getTextureSide();
    vec3 getTextureUp();
    void setTexture(GLuint tex);
    void setTextureSide(GLuint tex);
    void setTextureUp(GLuint tex);

    void setBoundingBox();
    void updateBoundingBox(mat4 rotation, GLfloat scale);
    vec3 getSize();
    void rotate(mat4 rotation);

    void draw(mat4 camMatrix, GLuint shader, float scale, mat4 rot);
    void drawOver(mat4 camMatrix, GLuint shader, float scale, mat4 rot, vec4(opac));
    void drawOn(mat4 camMatrix, GLuint shader, float scale, mat4 rot, Object* object);

    GLfloat getCorrHeightInt(int x, int z);
    GLfloat getRealHeight(GLfloat x, GLfloat z, Object* object, GLint flipped);

private:
    vec3 _position;
    vec3 _direction;
    vec3 _size;
    vec3 _speed;
    Model* _model;
    GLuint _texture;
    GLuint _textureSide;
    GLuint _textureUp, _textureUp0, _textureUp1, _textureUp2;
};

#endif
