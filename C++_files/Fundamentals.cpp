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
#include "Book.h"
#include <iostream>
using namespace std;

Fundamentals::Fundamentals(Camera* cam){
	camera = cam;
}

void Fundamentals::loadfiles(){
	void LoadTGATextureSimple(char const *filename, GLuint *tex);
	glEnable(GL_DEPTH_TEST);

	//init matrices
	camMatrix = camera->getCamMatrix();
	projectionMatrix = camera->getProj_matrix();
	glClearColor(0.9,0.9,1,0);

	//init shaders
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	programObj = loadShaders("obj.vert", "obj.frag");
	printError("load shader");

	//Load textures
	LoadTGATextureSimple("../textures/grass.tga", &grassTex);
	LoadTGATextureSimple("../textures/snow.tga", &snowTex);
	LoadTGATextureSimple("../textures/SkyBox512.tga", &skytex);
	LoadTGATextureSimple("../textures/Leather2.tga", &leatherTex);
	LoadTGATextureSimple("../textures/bilskissred.tga", &bilTex);
	LoadTGATextureSimple("../textures/water.tga", &truckTex);

	//Load Models
	backModel = LoadModelPlus("../Modeller/BookBack.obj");
	bottomModel = LoadModelPlus("../Modeller/BookBot.obj");
	topModel = LoadModelPlus("../Modeller/BookTop.obj");
	straightPageModel = LoadModelPlus("../Modeller/PageStraight.obj");
	bentPageModel =LoadModelPlus("../Modeller/PageBent.obj");
	skybox = LoadModelPlus("../Modeller/skybox.obj");
	carModel = LoadModelPlus("../Modeller/bilskiss.obj");
	truckModel = LoadModelPlus("../Modeller/LPTruck.obj");


	//Create Book Objects
	bookback = new Object(backPos, backModel, leatherTex);
	bottompage = new Object(bottomModel, leatherTex);
	toppage = new Object(topPos, topModel, leatherTex);
	pageStraight = new Object(straightPageModel, grassTex);
	pageBent = new Object(bentPos, bentPageModel, grassTex);
	pageBent->setTextureSide(snowTex);
	pageStraight->setTextureSide(snowTex);

	book = new Book(bottompage, bookback, toppage, pageStraight, pageBent);

	//Worlds Objects
	car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	truck = new Object(vec3(10.2f, 4.6f, 8.9f), truckModel, truckTex);
	truck->updateBoundingBox(Ry(M_PI/2), 3.0);


	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(programObj);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(pageShader);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(pageShader, "Tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(pageShader, "sideTex"), 1);
	printError("init shader");
}

void Fundamentals::cameraCollision(){
	// Book
	cameraCollisionFlag = camera->CheckCollision(bookback, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(bottompage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(toppage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(pageStraight, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(pageBent, cameraCollisionFlag);


	//object
	cameraCollisionFlag = camera->CheckCollision(truck, cameraCollisionFlag);
	//cameraCollisionFlag = camera->CheckCollision(car, cameraCollisionFlag);
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


  // glUseProgram(pageShader);
	// //Time variable
	// glUniform1f(glGetUniformLocation(pageShader, "t"), t);

	//Draw complete book
	book->draw(camMatrix, pageShader, t);

	//draw scene
	glUseProgram(programObj);

	//Car
	mat4 modelViewCar = T(car->getPosition().x, car->getPosition().y, car->getPosition().z);
	mat4 carTot = Mult(camMatrix, modelViewCar);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, car->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, carTot.m);
	DrawModel(car->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	//Fueltruck
	mat4 modelViewTruck = T(truck->getPosition().x, truck->getPosition().y, truck->getPosition().z);
	mat4 truckTot = Mult(camMatrix, Mult(Mult(modelViewTruck, S(3,3,3)), Ry(M_PI/2)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, truck->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, truckTot.m);
	DrawModel(truck->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

}
