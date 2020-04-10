#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <stdio.h>
#include "VectorUtils3.h"
#include "Lamp.h"

class LightSource
{
public:
    LightSource(vec3 position, vec3 colour);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getViewPosition();
    void setViewPosition(vec3 position);

    vec3 getColour();
    void setColour(vec3 color);

    vec3 getViewPos();
    void setViewPos(vec3 viewPos);

private:
    vec3 position;
    vec3 colour;
    vec3 viewPos;
};

#endif
