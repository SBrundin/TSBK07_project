#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
#include "Fundamentals.h"
#include "Object.h"
#include "Camera.h"

Fundamentals::Fundamentals(Camera* cam){
	camera = cam;
}

void Fundamentals::loadfiles(){
	void LoadTGATextureSimple(char const *filename, GLuint *tex);
	glEnable(GL_DEPTH_TEST);

	camMatrix = camera->getCamMatrix();
	projectionMatrix = camera->getProj_matrix();
	glClearColor(0.9,0.9,1,0);

	//init shaders
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	printError("load shader");

	//Load textures
	LoadTGATextureSimple("../textures/grass.tga", &grassTex);
	LoadTGATextureSimple("../textures/snow.tga", &snowTex);
	LoadTGATextureSimple("../textures/water.tga", &waterTex);
	LoadTGATextureSimple("../textures/SkyBox512.tga", &skytex);
	LoadTGATextureSimple("../textures/Leather2.tga", &leatherTex);

	//Create Objects
	car = new Object();
	car->setModel(LoadModelPlus("../Modeller/bilskiss.obj"));
	car->setBoundingBox();
	bookback = new Object(backPos, nullVec);
	bookback->setModel(LoadModelPlus("../Modeller/BookBack.obj"));
	bookback->setBoundingBox();
	bottompage = new Object();
	bottompage->setModel(LoadModelPlus("../Modeller/BookBot.obj"));
	bottompage->setBoundingBox();
	toppage = new Object(topPos, nullVec);
	toppage->setModel(LoadModelPlus("../Modeller/BookTop.obj"));
	toppage->setBoundingBox();
	pageStraight = new Object();
	pageStraight->setModel(LoadModelPlus("../Modeller/PageStraight.obj"));
	pageStraight->setBoundingBox();
	pageBent = new Object(bentPos, nullVec);
	pageBent->setModel(LoadModelPlus("../Modeller/PageBent.obj"));
	pageBent->setBoundingBox();
	skybox = LoadModelPlus("../Modeller/skybox.obj");

	glUseProgram(program);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

}

void Fundamentals::cameraCollision(){
	cameraCollisionFlag = camera->CheckCollision(car, cameraCollisionFlag);
	//cameraCollisionFlag = camera->CheckCollision(bookback, cameraCollisionFlag);
	//cameraCollisionFlag = camera->CheckCollision(bottompage, cameraCollisionFlag);
	//cameraCollisionFlag = camera->CheckCollision(toppage, cameraCollisionFlag);

	camera->checkFlag(cameraCollisionFlag);
	cameraCollisionFlag = false;

}

void Fundamentals::update(){
	cameraCollision();

	camMatrix = camera->getCamMatrix();

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
  t = t/1000;
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);

  printError("pre display");
  mat4 camMat2 = camMatrix;
  camMat2.m[3] = 0;
  camMat2.m[7] = 0;
  camMat2.m[11] = 0;
  camMat2.m[15] = 1;

  //Skybox with corr program
  glUseProgram(skyboxProg);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, skytex);
  glUniform1i(glGetUniformLocation(skyboxProg, "texUnit"), 0); // Texture unit 0
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);
  DrawModel(skybox, skyboxProg, "in_Position", NULL, "inTexCoord");



  glEnable(GL_DEPTH_TEST);
  glUseProgram(program);

	//Bottompage
	mat4 scale = S(1,1,1);
	mat4 modelViewBottom = T(bottompage->getPosition().x, bottompage->getPosition().y ,bottompage->getPosition().z);
	mat4 totalBottom = Mult(camMatrix, Mult(modelViewBottom, scale));

	//Back of the book
	mat4 modelViewBack = T(bookback->getPosition().x, bookback->getPosition().y ,bookback->getPosition().z);
	mat4 totalBack = Mult(camMatrix, Mult(modelViewBack, scale));

	//Toppage
	mat4 invRot = T(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
	mat4 totRot = Mult(Rz(t), invRot);
	mat4 transRot = T(rotationAxis.x, rotationAxis.y, rotationAxis.z);
	totRot = Mult(transRot, totRot);
	mat4 modelViewTop = T(toppage->getPosition().x, toppage->getPosition().y ,toppage->getPosition().z);
	totRot = Mult(modelViewTop, totRot);
	mat4 totalTop = Mult(camMatrix, totRot);

	//Drawing of the book
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, leatherTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBottom.m);
  DrawModel(bottompage->getModel(), program, "inPosition", "inNormal", "inTexCoord");
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBack.m);
	DrawModel(bookback->getModel(), program, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalTop.m);
	DrawModel(toppage->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//Time variable
	glUniform1f(glGetUniformLocation(program, "t"), t);

	//Car
	car->setPosition(rotationAxis);
	//mat4 rotTrans = T(0.0f, 0.0f, 0.0f);
	//mat4 rotCar = Mult(Rz(t), rotTrans);
	mat4 modelViewCar = T(car->getPosition().x, car->getPosition().y, car->getPosition().z );
	mat4 carTot = Mult(camMatrix, modelViewCar);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, carTot.m);
	DrawModel(car->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//Straight page
	mat4 pageSTot = T(pageStraight->getPosition().x, pageStraight->getPosition().y, pageStraight->getPosition().z );
 	pageSTot= Mult(camMatrix, pageSTot);
	glBindTexture(GL_TEXTURE_2D, snowTex);
	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, pageSTot.m);
	DrawModel(pageStraight->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//Bent page
	mat4 pageBTot = T(pageBent->getPosition().x, pageBent->getPosition().y, pageBent->getPosition().z );
	pageBTot = Mult(camMatrix, pageBTot);
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, pageBTot.m);
	DrawModel(pageBent->getModel(), program, "inPosition", "inNormal", "inTexCoord");


}
