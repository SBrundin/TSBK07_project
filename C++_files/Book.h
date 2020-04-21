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
    Book(Object* top, Object* firstPage, Object* secondPage, Object* frame, Object* pages);
    void draw(mat4 camMatrix, GLuint shader, GLfloat t);
    void browse(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* firstPage, GLuint pageNbr, char button);
    void makeRotation(GLfloat timer, GLuint page, mat4 camMatrix, GLuint shader, char button);
    mat4 generateRotationMat(vec3 rotationAxis, GLfloat time);

    void setBool(){
      _bool = !_bool;
    }

    bool getBool(){
      return _bool;
    }

    GLuint getCurrentPage(){
      return _currentPage;
    }

    GLuint getDirection(){
      if (_buttonPressed == 'l') return 1;
      if (_buttonPressed == 'r') return -1;
    }

    GLfloat getTimer(){
      return _timer;
    }


private:
    Object* _frame;
    Object* _top;
    Object* _firstPage;
    Object* _secondPage;
    Object* _pages;
    vec3 _firstPos = {0.21f, 3.133f, 0.0f};
    vec3 _topPos = {0.435f, 3.625f, 0.0f};
    vec3 _topPosOpen = {-29.565f, 2.375f, 0.0f};
    vec3 _firstOpen = {-29.065f, 2.768f, 0.0f};
    vec3 _rotationAxis = {-14.565f, -0.25f, 0.0f};
    GLuint _currentPage;
    GLuint _pageNbr;
    bool _bool;
    GLfloat _timer = 0;
    char _buttonPressed;
};

#endif
