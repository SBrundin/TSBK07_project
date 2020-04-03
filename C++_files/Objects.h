#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include "VectorUtils3.h"
class Objects
{
public:
    Objects();
    Objects(vec3 pos, vec3 direction);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

private:
    vec3 _position;
    vec3 _direction;
    vec3 _speed;
};

#endif
