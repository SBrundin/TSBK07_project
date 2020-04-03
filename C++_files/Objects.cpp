#include "Objects.h"
#include <stdio.h>
#include "VectorUtils3.h"

Objects::Objects()
{
    _position = vec3(0.0f, 0.0f, 0.0f);

    _direction = vec3(0.0f, 0.0f, 0.0f);
}
Objects::Objects(vec3 pos, vec3 direction)
{
    _position = pos + direction * 0.5;
    _direction = direction * 2;
}

vec3 Objects::getPosition()
{
    return _position;
}

void Objects::setPosition(vec3 position)
{
    _position = position;
}

vec3 Objects::getDirection()
{
    return _direction;
}

void Objects::setDirection(vec3 direction)
{
    _direction = direction;
}
