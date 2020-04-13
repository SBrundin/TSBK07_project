#include "Object.h"
#include "Book.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"

Book::Book(Object* bot, Object* back, Object* top, Object* ps, Object* pb){
  _bottom = bot;
  _back = back;
  _top = top;
  _pageBent = pb;
  _pageStraight = ps;
}

void Book::draw(mat4 camMatrix, GLuint shader, GLfloat t){
  //Bottompage
	mat4 scale = S(1,1,1);
	mat4 modelViewBottom = T(_bottom->getPosition().x, _bottom->getPosition().y ,_bottom->getPosition().z);
	mat4 totalBottom = Mult(camMatrix, Mult(modelViewBottom, scale));

	//Back of the book
	mat4 modelViewBack = T(_back->getPosition().x, _back->getPosition().y ,_back->getPosition().z);
	mat4 totalBack = Mult(camMatrix, Mult(modelViewBack, scale));

	//Toppage
  mat4 invRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
	mat4 totRot = Mult(Rz(t), invRot);
	mat4 transRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
	totRot = Mult(transRot, totRot);
	mat4 modelViewTop = T(_top->getPosition().x, _top->getPosition().y ,_top->getPosition().z);
	totRot = Mult(modelViewTop, totRot);
  mat4 totalTop = Mult(camMatrix, totRot);

	//Drawing of the book
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _top->getTexture());
  glUniform1i(glGetUniformLocation(shader, "bookTex"), 0);
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBottom.m);
  DrawModel(_bottom->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
  glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalBack.m);
	DrawModel(_back->getModel(), shader, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
	DrawModel(_top->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

  //Straight page
	mat4 pageSTot = T(_pageStraight->getPosition().x, _pageStraight->getPosition().y, _pageStraight->getPosition().z );
 	pageSTot= Mult(camMatrix, pageSTot);
	glBindTexture(GL_TEXTURE_2D, _pageStraight->getTexture());
	glUniform1i(glGetUniformLocation(shader, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, pageSTot.m);
	DrawModel(_pageStraight->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

	//Bent page
	mat4 pageBTot = T(_pageBent->getPosition().x, _pageBent->getPosition().y, _pageBent->getPosition().z );
	pageBTot = Mult(camMatrix, pageBTot);
	glBindTexture(GL_TEXTURE_2D, _pageBent->getTexture());
	glUniform1i(glGetUniformLocation(shader, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(shader, "mdlMatrix"), 1, GL_TRUE, pageBTot.m);
	DrawModel(_pageBent->getModel(), shader, "inPosition", "inNormal", "inTexCoord");

}
