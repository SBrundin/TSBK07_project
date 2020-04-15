#include "Object.h"
#include "Book.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "MicroGlut.h"
#include <iostream>
using namespace std;

Book::Book(Object* bot, Object* back, Object* top, Object* ps, Object* pb){
  _bottom = bot;
  _back = back;
  _top = top;
  _pageBent = pb;
  _pageStraight = ps;
  _currentPage = 1;
  _bool = false;
}

void Book::draw(mat4 camMatrix, GLuint shader, GLfloat t){

  glUseProgram(shader);
  glUniform1f(glGetUniformLocation(shader, "t"), t);

	//INITIATES THE BROWSING
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());

  if (glutKeyIsDown('n') && getBool() == false && _currentPage != 3)
    {
      _timer = 0;
      setBool();
      _buttonPressed = 'l';
    }
  if (glutKeyIsDown('m') && getBool() == false && _currentPage != 1)
      {
        _timer = 0;
        setBool();
        _buttonPressed = 'r';
      }
  if (getBool() == true) {

      //EXECUTES THE ROTATION
      makeRotation(_timer, _currentPage, camMatrix, shader, _buttonPressed);
      }

  else{

  //DRAWS TOP PAGE WHEN NOT ROTATING
  mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);
  mat4 totalTop = Mult(camMatrix, modelViewTop);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
  DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //DRAWS BENT PAGE WHEN NOT ROTATING
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _pageBent->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _pageBent->getTextureSide());
  mat4 modelViewBent = T(_pageBent->getPosition().x, _pageBent->getPosition().y, _pageBent->getPosition().z );
  mat4 totalBent = Mult(camMatrix, modelViewBent);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBent.m);
  DrawModel(_pageBent->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  }

  //DRAWS REST OF THE STATIC COMONENTS
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());

  //BOTTOMPAGE
  mat4 modelViewBottom = T(_bottom->getPosition().x, _bottom->getPosition().y ,_bottom->getPosition().z);
  mat4 totalBottom = Mult(camMatrix, modelViewBottom);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBottom.m);
  DrawModel(_bottom->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //BACK OF THE BOOK
  mat4 modelViewBack = T(_back->getPosition().x, _back->getPosition().y ,_back->getPosition().z);
  mat4 totalBack = Mult(camMatrix, modelViewBack);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBack.m);
  DrawModel(_back->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //STRAIGHT PAGE
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _pageStraight->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _pageStraight->getTextureSide());
  mat4 modelViewStraight = T(_pageStraight->getPosition().x, _pageStraight->getPosition().y, _pageStraight->getPosition().z );
 	mat4 totalStraight= Mult(camMatrix, modelViewStraight);
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalStraight.m);
	DrawModel(_pageStraight->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}

mat4 Book::generateRotationMat(vec3 rotationAxis, GLfloat time){
  mat4 invRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
  mat4 totRot = Mult(Rz(time), invRot);
  mat4 transRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
  totRot = Mult(transRot, totRot);
  return totRot;
}


void Book::browse(mat4 camMatrix, GLuint shader, GLfloat time, Object* top, Object* second, GLuint pageNbr, char button){
  mat4 totRot;
  mat4 totalSec;
  mat4 totalTop;
  mat4 modelViewTop = T(top->getPosition().x, top->getPosition().y ,top->getPosition().z);
  mat4 modelViewSec = T(second->getPosition().x, second->getPosition().y ,second->getPosition().z);

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
  DrawModel(top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _pageBent->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _pageBent->getTextureSide());
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalSec.m);
  DrawModel(second->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}


void Book::makeRotation(GLfloat timer, GLuint currentPage, mat4 camMatrix, GLuint shader, char button){

  //TOP ROTATION FORWARD
  if (timer <= 3.13 && currentPage == 1 && button == 'l')
  {
    _pageNbr = 1;
    browse(camMatrix, shader, _timer, _top, _pageBent, _pageNbr, button);
    _timer = _timer +0.05;
  }
  else if(timer > 3.13 && currentPage == 1 && button == 'l'){
    _top->setPosition(_topPosOpen);
    setBool();
    _currentPage++;
  }
  //TOP ROTATION BACKWARD
  else if (timer <= 3.13 && currentPage == 2 && button == 'r')
  {
    _pageNbr = 1;
    browse(camMatrix, shader, _timer, _top, _pageBent, _pageNbr, button);
    _timer = _timer +0.05;
  }

  else if (timer > 3.13 && currentPage == 2 && button == 'r'){
    _top->setPosition(_topPos);
    setBool();
    _currentPage--;
  }
  //BENT PAGE ROTATION FORWARD
  else if (timer <= 3.13 && currentPage == 2 && button == 'l')
  {
    _pageNbr = 2;
    browse(camMatrix, shader, _timer, _top, _pageBent, _pageNbr, button);
    _timer = _timer +0.05;
  }
  else if(timer > 2.5 && currentPage == 2 && button == 'l' ){
    _pageBent->setPosition(_bentOpen);
    setBool();
    _currentPage++;
  }

  //BENT PAGE ROTATION BACKWARD
  else if (timer <= 3.13 && currentPage == 3 && button == 'r')
  {
    _pageNbr = 2;
    browse(camMatrix, shader, _timer, _top, _pageBent, _pageNbr, button);
    _timer = _timer +0.05;
  }

  else if (timer > 3.13 && currentPage == 3 && button == 'r'){
    _pageBent->setPosition(_bentPos);
    setBool();
    _currentPage--;
  }

}
