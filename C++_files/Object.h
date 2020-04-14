#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
class Object
{
public:
    Object();
    Object(Model* model, GLuint tex);
    Object(vec3 pos, Model* model, GLuint tex);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    Model* getModel();
    void setModel(Model* model);

    GLuint getTexture();
    void setTexture(GLuint tex);

    void setBoundingBox();
    vec3 getSize();

private:
    vec3 _position;
    vec3 _direction;
    vec3 _size;
    vec3 _speed;
    Model* _model;
    GLuint _texture;
};

#endif
