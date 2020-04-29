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
    void draw(mat4 camMatrix, GLuint shader, GLfloat t, vec3 viewPos);
    void browse(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* firstPage, GLuint pageNbr, char button);
    void makeRotation(GLfloat timer, GLuint page, mat4 camMatrix, GLuint shader, char button);
    mat4 generateRotationMat(vec3 rotationAxis, GLfloat time);

    void setRotationBool(){
      _rotationBool = !_rotationBool;
    }

    bool getRotationBool(){
      return _rotationBool;
    }

    void setFadeBool(){
      _fadeBool = !_fadeBool;
    }

    bool getFadeBool(){
      return _fadeBool;
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

    void setTimer(GLfloat t){
      _timer = t;
    }

    void increaseTimer(GLfloat t){
      _timer += t;
    }

    void decreaseTimer(GLfloat t){
      _timer -= t;
    }

private:
    Object* _frame;
    Object* _top;
    Object* _firstPage;
    Object* _secondPage;
    Object* _pages;
    vec3 _firstPos = {0.108f, 3.133f, 0.0f};
    vec3 _topPos = {0.435f, 3.625f, 0.0f};
    vec3 _topPosOpen = {-29.565f, 2.375f, 0.0f};
    vec3 _firstOpen = {-28.962f, 2.767f, 0.0f};
    vec3 _secondPos = {0.06f, 2.78f, 0.0f};
    vec3 _rotationAxistop = {-15.0f, -0.625f, 0.0f};
    vec3 _rotationAxisp1 = {-14.535f, -0.183f, 0.0f};
    vec3 _pagesPos = {-0.315f, 0.173f, 0.0f};
    GLuint _currentPage;
    GLuint _pageNbr;
    bool _rotationBool;
    bool _fadeBool;
    GLfloat _timer = 0;
    char _buttonPressed;
    mat4 _openRot = Rz(0);
};

#endif
