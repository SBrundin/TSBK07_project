#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
class Object
{
public:
    Object();
    Object(vec3 pos, vec3 direction);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    Model* getModel();
    void setModel(Model* model);

    void setBoundingBox();
    vec3 getSize();

private:
    vec3 _position;
    vec3 _direction;
    vec3 _size;
    vec3 _speed;
    Model* _model;
};

#endif
