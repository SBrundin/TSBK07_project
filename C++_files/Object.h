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

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    Model* getModel();
    void setModel(Model* model);

    GLuint getTexture();
    GLuint getTextureSide();
    GLuint getTextureUp();
    void setTexture(GLuint tex);
    void setTextureSide(GLuint tex);
    void setTextureUp(GLuint tex);

    void setBoundingBox();
    void updateBoundingBox(mat4 rotation, GLfloat scale);
    vec3 getSize();

    void draw(mat4 camMatrix, GLuint shader, float scale, mat4 rot);

    GLfloat getCorrHeightInt(int x, int z);
    GLfloat getRealHeight(GLfloat x, GLfloat z);

private:
    vec3 _position;
    vec3 _direction;
    vec3 _size;
    vec3 _speed;
    Model* _model;
    GLuint _texture;
    GLuint _textureSide;
    GLuint _textureUp;
};

#endif
