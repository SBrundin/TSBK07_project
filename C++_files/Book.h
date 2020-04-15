#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include "Object.h"
#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "loadobj.h"
class Book
{
public:
    Book(Object* bot, Object* back, Object* top, Object* ps, Object* pb);
    void draw(mat4 camMatrix, GLuint shader, GLfloat t);
    void browseForward(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* second, GLuint pageNbr);
    void browseBackward(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* second, GLuint pageNbr);
    void makeRotation(GLfloat timer, GLuint page, mat4 camMatrix, GLuint shader, char button);
    void setBool(){
      _bool = !_bool;
    }
    bool getBool(){
      return _bool;
    };

private:
    Object* _bottom;
    Object* _back;
    Object* _top;
    Object* _pageBent;
    Object* _pageStraight;
    vec3 nullVec = {0.0f, 0.0f, 0.0f};
    vec3 _backPos = {-15.17f, 2.5f, 0.0f};
    vec3 _bentPos = {0.0f, 5.0f, 0.0f};
    vec3 _straightPos = {0.0f, 1.0f, 0.0f};
    vec3 _topPos = {0.0f, 5.3f, 0.0f};
    vec3 rotationAxis = {-15.0f, 0.0f, 0.0f};
    vec3 topPosOpen = {-29.7f, 5.0f, 0.0f};
    vec3 bentOpen = {-29.5f, 5.6f, 0.0f};
    GLuint _currentPage;
    GLuint _pageNbr;
    bool _bool;
    GLfloat _timer;
    char _buttonPressed;
};

#endif
