#include "Object.h"
#include "Book.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "MicroGlut.h"
#include <iostream>
using namespace std;


Book::Book(Object* top, Object* firstPage, Object* secondPage, Object* frame, Object* pages){
  _frame = frame;
  _top = top;
  _firstPage = firstPage;
  _secondPage = secondPage;
  _pages = pages;
  _currentPage = 1;
  _rotationBool = false;
  _fadeBool = false;
  _timer = 0;
  _top->setPosition(_topPos);
  _firstPage->setPosition(_firstPos);
  _secondPage->setPosition(_secondPos);
}

void Book::draw(mat4 camMatrix, GLuint shader, GLfloat t){

  glUseProgram(shader);

  if (glutKeyIsDown('n') && getRotationBool() == false && _currentPage != 3)
    {
      setRotationBool();
      setFadeBool();
      _buttonPressed = 'l';
    }

  if (glutKeyIsDown('m') && getRotationBool() == false && _currentPage != 1)
      {
        setRotationBool();
        setFadeBool();
        _buttonPressed = 'r';
      }
  if ((getRotationBool() == true) && !getFadeBool()) {

      //EXECUTES THE ROTATION
      makeRotation(_timer, _currentPage, camMatrix, shader, _buttonPressed);
      }

  else{
	//INITIATES THE BROWSING
    if (_currentPage == 1){
      glUniform1i(glGetUniformLocation(shader, "ID"), 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _top->getTexture());
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, _top->getTextureSide());
      mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);
      mat4 totalTop = Mult(camMatrix, modelViewTop);
      glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
      DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
      //First page
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _firstPage->getTexture());
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureSide());
      mat4 modelViewfirst = T(_firstPage->getPosition().x, _firstPage->getPosition().y, _firstPage->getPosition().z );
      mat4 totalfirst = Mult(camMatrix, modelViewfirst);
      glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalfirst.m);
      DrawModel(_firstPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
      //Second pages
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _secondPage->getTexture());
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureSide());
      mat4 modelViewSecond = T(_secondPage->getPosition().x, _secondPage->getPosition().y, _secondPage->getPosition().z);
      mat4 totalSecond = Mult(camMatrix, modelViewSecond);
      glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalSecond.m);
      DrawModel(_secondPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
    }
  else if (_currentPage == 2){
    //Cover top
    glUniform1i(glGetUniformLocation(shader, "ID"), 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _top->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().x);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().y);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().z);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _top->getTextureSide());
    mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);
    mat4 totalTop = Mult(camMatrix, modelViewTop);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
    DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
    //First page
    glUniform1i(glGetUniformLocation(shader, "ID"), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().x);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().y);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureUp().z);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureSide());
    mat4 modelViewfirst = T(_firstPage->getPosition().x, _firstPage->getPosition().y, _firstPage->getPosition().z );
    mat4 totalfirst = Mult(camMatrix, modelViewfirst);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalfirst.m);
    DrawModel(_firstPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
    //Second pages
    glUniform1i(glGetUniformLocation(shader, "ID"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTexture());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureSide());
    mat4 modelViewSecond = T(_secondPage->getPosition().x, _secondPage->getPosition().y, _secondPage->getPosition().z );
    mat4 totalSecond = Mult(camMatrix, modelViewSecond);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalSecond.m);
    DrawModel(_secondPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  }
  else if (_currentPage == 3){
    //Cover top
    glUniform1i(glGetUniformLocation(shader, "ID"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _top->getTexture());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _top->getTextureSide());
    mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);
    mat4 totalTop = Mult(camMatrix, modelViewTop);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
    DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
    //First page
    glUniform1i(glGetUniformLocation(shader, "ID"), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().x);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().y);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().x);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureSide());
    mat4 modelViewfirst = T(_firstPage->getPosition().x, _firstPage->getPosition().y, _firstPage->getPosition().z );
    mat4 totalfirst = Mult(camMatrix, modelViewfirst);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalfirst.m);
    DrawModel(_firstPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
    //Second pages
    glUniform1i(glGetUniformLocation(shader, "ID"), 3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().x);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().y);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureUp().z);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _secondPage->getTextureSide());
    mat4 modelViewSecond = T(_secondPage->getPosition().x, _secondPage->getPosition().y, _secondPage->getPosition().z );
    mat4 totalSecond = Mult(camMatrix, modelViewSecond);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalSecond.m);
    DrawModel(_secondPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  }
  else{
    std::cout << "Fel sid ID" << std::endl;
    }
  }

  //STATIC PAGES
  glUniform1i(glGetUniformLocation(shader, "ID"), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _pages->getTexture());
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, _pages->getTextureSide());
  mat4 modelViewPages = T(_pages->getPosition().x, _pages->getPosition().y ,_pages->getPosition().z);
  mat4 totalPages = Mult(camMatrix, modelViewPages);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalPages.m);
  DrawModel(_pages->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  //FRAME OF THE BOOK
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _frame->getTexture());
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, _frame->getTextureSide());
  mat4 modelViewFrame = T(_frame->getPosition().x, _frame->getPosition().y, _frame->getPosition().z );
 	mat4 totalFrame= Mult(camMatrix, modelViewFrame);
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalFrame.m);
	DrawModel(_frame->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}

mat4 Book::generateRotationMat(vec3 rotationAxis, GLfloat time){
  mat4 invRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
  mat4 totRot = Mult(Rz(time), invRot);
  mat4 transRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
  totRot = Mult(transRot, totRot);
  return totRot;
}


void Book::browse(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* firstPage, GLuint pageNbr, char button){
  mat4 totRot;
  mat4 totalSec;
  mat4 totalTop;
  mat4 modelViewTop = T(top->getPosition().x, top->getPosition().y ,top->getPosition().z);
  mat4 modelViewSec = T(firstPage->getPosition().x, firstPage->getPosition().y ,firstPage->getPosition().z);

  if(button == 'l'){
    totRot = generateRotationMat(_rotationAxis, time);
  }
  else{
    totRot = generateRotationMat(ScalarMult(_rotationAxis, -1), -time);
  }

  if (pageNbr == 1){
  	totRot = Mult(modelViewTop, totRot);
    totalTop = Mult(camMatrix, totRot);
    totalSec = Mult(camMatrix, modelViewSec);
  }
  else{
    totRot = Mult(modelViewSec, totRot);
    totalSec = Mult(camMatrix, totRot);
    totalTop = Mult(camMatrix, modelViewTop);
  }

  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
  glUniform1i(glGetUniformLocation(shader, "ID"), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, _top->getTextureSide());
  DrawModel(top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _firstPage->getTexture());
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, _firstPage->getTextureSide());
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalSec.m);
  DrawModel(firstPage->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}


void Book::makeRotation(GLfloat timer, GLuint currentPage, mat4 camMatrix, GLuint shader, char button){

  //TOP ROTATION FORWARD
  if (timer <= 3.13 && currentPage == 1 && button == 'l')
  {
    _pageNbr = 1;
    browse(camMatrix, shader, _timer, _top, _firstPage, _pageNbr, button);
    _timer = _timer +0.05;
  }
  else if(timer > 3.13 && currentPage == 1 && button == 'l'){
    _top->setPosition(_topPosOpen);
    setRotationBool();
    _currentPage++;
    _timer = 0;
  }
  //TOP ROTATION BACKWARD
  else if (timer <= 3.13 && currentPage == 2 && button == 'r')
  {
    _pageNbr = 1;
    browse(camMatrix, shader, _timer, _top, _firstPage, _pageNbr, button);
    _timer = _timer +0.05;
  }

  else if (timer > 3.13 && currentPage == 2 && button == 'r'){
    _top->setPosition(_topPos);
    setRotationBool();
    _currentPage--;
    _timer = 0;
  }
  //FIRST PAGE ROTATION FORWARD
  else if (timer <= 3.13 && currentPage == 2 && button == 'l')
  {
    _pageNbr = 2;
    browse(camMatrix, shader, _timer, _top, _firstPage, _pageNbr, button);
    _timer = _timer +0.05;
  }
  else if(timer > 2.5 && currentPage == 2 && button == 'l' ){
    _firstPage->setPosition(_firstOpen);
    setRotationBool();
    _currentPage++;
    _timer = 0;
  }

  //FIRST PAGE ROTATION BACKWARD
  else if (timer <= 3.13 && currentPage == 3 && button == 'r')
  {
    _pageNbr = 2;
    browse(camMatrix, shader, _timer, _top, _firstPage, _pageNbr, button);
    _timer = _timer +0.05;

  }

  else if (timer > 3.13 && currentPage == 3 && button == 'r'){
    _firstPage->setPosition(_firstPos);
    setRotationBool();
    _currentPage--;
    _timer = 0;
  }

}
