#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include "Object.h"
#include "VectorUtils3.h"
#include "loadobj.h"
class Book
{
public:
    Book(Object* bot, Object* back, Object* top, Object* ps, Object* pb);
    void draw(mat4 camMatrix, GLuint shader, GLfloat t);

private:
    Object* _bottom;
    Object* _back;
    Object* _top;
    Object* _pageBent;
    Object* _pageStraight;
    vec3 nullVec = {0.0f, 0.0f, 0.0f};
    vec3 _backPos = {-15.17f, 2.5f, 0.0f};
    vec3 _bentPos = {0.0f, 4.0f, 0.0f};
    vec3 _straightPos = {0.0f, 1.0f, 0.0f};
    vec3 _topPos = {0.0f, 5.3f, 0.0f};
    vec3 rotationAxis = {-15.0f, 0.0f, 0.0f};

};

#endif
