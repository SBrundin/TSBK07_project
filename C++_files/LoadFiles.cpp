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
#include "LoadFiles.h"
#include "Camera.h"
#include <string>


void LoadFiles::initiate(mat4 projectionMatrix){

  void LoadTGATextureSimple(char const *filename, GLuint *tex);

	// GL inits
	glClearColor(0.5,0.5,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	glUseProgram(program);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(pageShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	LoadFiles::loadskybox();

  LoadTGATextureSimple("../textures/grass.tga", &grassTex);
  LoadTGATextureSimple("../textures/snow.tga", &snowTex);
  LoadTGATextureSimple("../textures/water.tga", &waterTex);
  //LoadTGATextureSimple("../textures/grass.tga", &skytex);

  //skybox = LoadModelPlus("../Modeller/SkyboxKUB.obj");
  boktop = LoadModelPlus("../Modeller/Boktop.obj");
  bokrygg = LoadModelPlus("../Modeller/bokrygg.obj");
}

void LoadFiles::update(mat4 projectionMatrix, mat4 camMatrix){

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
  t = t/1000;
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);

  printError("pre display");
  // mat4 camMatrix = lookAt(cam.x, cam.y, cam.z,
  //                   lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
  //                   v.x, v.y, v.z);
  mat4 camMat2 = camMatrix;
  camMat2.m[3] = 0;
  camMat2.m[7] = 0;
  camMat2.m[11] = 0;
  camMat2.m[15] = 1;



  //Skybox with corr program
  glUseProgram(skyboxProg);
  glDisable(GL_DEPTH_TEST);

	timer += 0.01;
	timer = std::min(timer, 1.0f);


  //glBindTexture(GL_TEXTURE_2D, skytex);
  glUniform1i(glGetUniformLocation(skyboxProg, "texUnit"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(skyboxProg, "test"), 1); // Texture unit 1
glUniform1i(glGetUniformLocation(skyboxProg, "page"), page);
	glUniform1f(glGetUniformLocation(skyboxProg, "timer"), timer);
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

	if (glutKeyIsDown(GLUT_KEY_LEFT))
	{
		if (page == 0)
	{
		page = 1;
		timer = 0.0f;
	}
	else if (page == 1)
	{
		page = 0;
		timer = 0.0f;
	}
	}
	
//Camera vectors
  glEnable(GL_DEPTH_TEST);
  mat4 scale = S(5,5,5);
  mat4 modelViewBook = T(Bookx, Booky, Bookz);
  mat4 totalBook = Mult(camMatrix, Mult(modelViewBook, scale));

  glUseProgram(program);
  //Draw the bunny
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grassTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook.m);
  DrawModel(boktop, program, "inPosition", "inNormal", "inTexCoord");
  DrawModel(bokrygg, program, "inPosition", "inNormal", "inTexCoord");
  glUniform1f(glGetUniformLocation(program, "t"), t);

  GLfloat upperPage = Booky+2.5*5;
  mat4 modelViewBook2 = T(Bookx, upperPage, Bookz);
  mat4 totalBook2 = Mult(camMatrix, modelViewBook2);
  totalBook2 = Mult(camMatrix, Mult(modelViewBook2,scale));

  glBindTexture(GL_TEXTURE_2D, waterTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook2.m);
  DrawModel(boktop, program, "inPosition", "inNormal", "inTexCoord");

}

void LoadFiles::loadskybox()
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
