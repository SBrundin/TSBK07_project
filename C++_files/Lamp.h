#ifndef LAMP_H
#define LAMP_H

#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "Object.h"

class Lamp
{
public:
    Lamp(Object* obj);

    vec3 getPosition();
    void setPosition(vec3 position);

    Object* getObject();
    void setObject(Object* obj);

    vec3 getColour();
    void setColour(vec3 light);

private:
    vec3 position;
    vec3 colour;
    Object* object;
};

#endif
