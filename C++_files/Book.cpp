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
  //Bottompage
	mat4 scale = S(1,1,1);
	mat4 modelViewBottom = T(_bottom->getPosition().x, _bottom->getPosition().y ,_bottom->getPosition().z);
	mat4 totalBottom = Mult(camMatrix, modelViewBottom);

	//Back of the book
	mat4 modelViewBack = T(_back->getPosition().x, _back->getPosition().y ,_back->getPosition().z);
	mat4 totalBack = Mult(camMatrix, modelViewBack);

  mat4 modelViewStraight = T(_pageStraight->getPosition().x, _pageStraight->getPosition().y, _pageStraight->getPosition().z );
 	mat4 totalStraight= Mult(camMatrix, modelViewStraight);

  mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);

  mat4 modelViewBent = T(_pageBent->getPosition().x, _pageBent->getPosition().y, _pageBent->getPosition().z );


	//Drawing of the book
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
  if (glutKeyIsDown(GLUT_KEY_LEFT) && getBool() == false && _currentPage != 3)
    {
      _timer = t/t -1;
      setBool();
      _buttonPressed = 'l';
    }
  if (glutKeyIsDown(GLUT_KEY_RIGHT) && getBool() == false && _currentPage != 1)
      {
        _timer = t/t -1;
        setBool();
        _buttonPressed = 'r';
      }
  if (getBool() == true) {
        makeRotation(_timer, _currentPage, camMatrix, shader, _buttonPressed);
    }

    else{

  mat4 totalTop = Mult(camMatrix, modelViewTop);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
}

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
	DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  glUniform1i(glGetUniformLocation(shader, "bookTex"), 0);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBottom.m);
  DrawModel(_bottom->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBack.m);
  DrawModel(_back->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //Straight page
	glBindTexture(GL_TEXTURE_2D, _pageStraight->getTexture());
	glUniform1i(glGetUniformLocation(shader, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalStraight.m);
	DrawModel(_pageStraight->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //Bent page
  glBindTexture(GL_TEXTURE_2D, _pageBent->getTexture());
  glUniform1i(glGetUniformLocation(shader, "bookTex"), 0); // Texture unit 0
  mat4 totalBent = Mult(camMatrix, modelViewBent);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBent.m);
  DrawModel(_pageBent->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

}


void Book::browseForward(mat4 camMatrix, GLuint shader, GLfloat time, Object* page, GLuint pageNbr){
	mat4 invRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
	mat4 totRot = Mult(Rz(time), invRot);
	mat4 transRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
	totRot = Mult(transRot, totRot);
	mat4 modelViewTop = T(page->getPosition().x, page->getPosition().y ,page->getPosition().z);
	totRot = Mult(modelViewTop, totRot);
  mat4 totalTop = Mult(camMatrix, totRot);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
	DrawModel(page->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}

void Book::browseBackward(mat4 camMatrix, GLuint shader, GLfloat time, Object* page, GLuint pageNbr){
	mat4 invRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
	mat4 totRot = Mult(Rz(-time), invRot);
	mat4 transRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
	totRot = Mult(transRot, totRot);
	mat4 modelViewTop = T(page->getPosition().x, page->getPosition().y ,page->getPosition().z);
	totRot = Mult(modelViewTop, totRot);
  mat4 totalTop = Mult(camMatrix, totRot);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
	DrawModel(page->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
}

void Book::makeRotation(GLfloat timer, GLuint currentPage, mat4 camMatrix, GLuint shader, char button){

  //top rotation forward
  if (timer <= 3.13 && currentPage == 1 && button == 'l')
  {
    _pageNbr = 1;
    browseForward(camMatrix, shader, _timer, _top, _pageNbr);
    _timer = _timer +0.05;
  }
  else if(timer > 3.13 && currentPage == 1 && button == 'l'){
    _top->setPosition(topPosOpen);
    setBool();
    _currentPage++;
  }
  //top rotation backward
  else if (timer <= 3.13 && currentPage == 2 && button == 'r')
  {
    _pageNbr = 1;
    browseBackward(camMatrix, shader, _timer, _top, _pageNbr);
    _timer = _timer +0.05;
  }

  else if (timer > 3.13 && currentPage == 2 && button == 'r'){
    _top->setPosition(_topPos);
      setBool();
      _currentPage--;
  }

  //pageBent rotation forward
  else if (timer <= 3.13 && currentPage == 2 && button == 'l')
  {
    _pageNbr = 2;
    browseForward(camMatrix, shader, _timer, _pageBent, _pageNbr);
    _timer = _timer +0.05;
  }
  else if(timer > 3.13 && currentPage == 2 && button == 'l' ){
    _pageBent->setPosition(straightPosOpen);
    setBool();
    _currentPage++;
  }

  //pageBent rotation backward
  else if (timer <= 3.13 && currentPage == 3 && button == 'r')
  {
    _pageNbr = 2;
    browseBackward(camMatrix, shader, _timer, _pageBent, _pageNbr);
    _timer = _timer +0.05;
  }

  else if (timer > 3.13 && currentPage == 3 && button == 'r'){
    _pageBent->setPosition(_bentPos);
      setBool();
      _currentPage--;
  }

}
