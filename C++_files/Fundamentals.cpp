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
	//LoadTGATextureSimple("../textures/Paper.tga", &paperTex);
	Fundamentals::loadskybox();
	LoadTGATextureSimple("../textures/Leather2.tga", &leatherTex);
	LoadTGATextureSimple("../textures/bilskissred.tga", &bilTex);

	//Load Models
	carModel = LoadModelPlus("../Modeller/bilskiss.obj");
	backModel = LoadModelPlus("../Modeller/BookBack.obj");
	bottomModel = LoadModelPlus("../Modeller/BookBot.obj");
	topModel = LoadModelPlus("../Modeller/BookTop.obj");
	straightPageModel = LoadModelPlus("../Modeller/PageStraight.obj");
	bentPageModel =LoadModelPlus("../Modeller/PageBent.obj");


	//Create Objects
	car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	bookback = new Object(backPos, backModel, leatherTex);
	bottompage = new Object(bottomModel, leatherTex);
	toppage = new Object(topPos, topModel, leatherTex);
	pageStraight = new Object(straightPageModel, grassTex);
	pageBent = new Object(bentPos, bentPageModel, grassTex);
	pageBent->setTextureSide(snowTex);
	pageStraight->setTextureSide(snowTex);

	//Book
	book = new Book(bottompage, bookback, toppage, pageStraight, pageBent);

	printError("init shader");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(programObj);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(pageShader);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(pageShader, "Tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(pageShader, "sideTex"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pageStraight->getTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pageBent->getTexture());
}

void Fundamentals::cameraCollision(){
	cameraCollisionFlag = camera->CheckCollision(car, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(bookback, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(bottompage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(toppage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(pageStraight, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(pageBent, cameraCollisionFlag);

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
	glUniform1i(glGetUniformLocation(skyboxProg, "tex0"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(skyboxProg, "tex1"), 1); // Texture unit 1
	glUniform1i(glGetUniformLocation(skyboxProg, "ID"), book->getCurrentPage());
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);

	for (unsigned int i = 0; i < 6; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skytex[i].texID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skytex[i + 6].texID);

		DrawModel(skybox[i], skyboxProg, "inPosition", NULL, "inTexCoord");
	}

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

}

void Fundamentals::loadskybox()
{
	//glActiveTexture(GL_TEXTURE0);

	std::string	skytextures[6*2] =
	{
		"../textures/skybox0/left.tga",
		"../textures/skybox0/right.tga",
		"../textures/skybox0/top.tga",
		"../textures/skybox0/bottom.tga",
		"../textures/skybox0/front.tga",
		"../textures/skybox0/back.tga",

		"../textures/skyboxdebug/left.tga",
		"../textures/skyboxdebug/right.tga",
		"../textures/skyboxdebug/top.tga",
		"../textures/skyboxdebug/bottom.tga",
		"../textures/skyboxdebug/front.tga",
		"../textures/skyboxdebug/back.tga"
	};

	std::string filename[6] =
{
	"../Modeller/skybox/side0.obj",
	"../Modeller/skybox/side1.obj",
	"../Modeller/skybox/side2.obj",
	"../Modeller/skybox/side3.obj",
	"../Modeller/skybox/side4.obj",
	"../Modeller/skybox/side5.obj"
};

	for (unsigned int i = 0; i < 6*2; i++)
	{
		printf("Loading texture %s\n", skytextures[i].c_str());
		LoadTGATexture(skytextures[i].c_str(), &skytex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	};

	for (unsigned int i = 0; i < 6; i++)
	{
		skybox[i] = LoadModelPlus(filename[i].c_str());
	}

}
