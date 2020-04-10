#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
class Book
{
public:
    Book(Model* bottom, Model* back, Model* top, Model* pageBent, Model* pageStraight);
    Book(vec3 pos, Model* bottom, Model* back, Model* top, Model* pageBent, Model* pageStraight);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    void setBoundingBox();
    vec3 getSize();

private:
    vec3 _position;
    vec3 _size;
    vec3 _speed;
    Model* _bottom;
    Model* _back;
    Model* _top;
    Model* _pageBent;
    Model* _pageStraight;
};

#endif
