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
    void draw(mat4 camMatrix, GLuint Covershader, GLuint shader, GLfloat t);
    void browse(mat4 camMatrix, GLuint shader, GLfloat time);
    void setBool();
    bool getBool();

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
    GLuint _currentPage;
    bool _bool;

};

#endif
