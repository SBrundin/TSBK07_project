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

	//Create Objects
	car = new Object();
	car->setModel(LoadModelPlus("../Modeller/bilskiss.obj"));
	car->setBoundingBox();
	bookback = new Object();
	bookback->setModel(LoadModelPlus("../Modeller/bokrygg.obj"));
	bookback->setBoundingBox();
	bottompage = new Object();
	bottompage->setModel(LoadModelPlus("../Modeller/Boktop.obj"));
	bottompage->setBoundingBox();
	toppage = new Object();
	toppage->setModel(LoadModelPlus("../Modeller/Boktop.obj"));
	toppage->setBoundingBox();

	skybox = LoadModelPlus("../Modeller/skybox.obj");

	glUseProgram(program);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

}

void Fundamentals::initiate(mat4 projectionMatrix){

}

void Fundamentals::update(){
	bool flag1 = camera->CheckCollision(car);
	//bool flag2 = camera->CheckCollision(bookback);
	//bool flag3 = camera->CheckCollision(bottompage);
	//bool flag4 = camera->CheckCollision(toppage);
	camera->checkFlag(flag1);
	//camera->checkFlag(flag2);
	//camera->checkFlag(flag3);
	//camera->checkFlag(flag4);

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


	GLfloat myRotX[]=
							{ 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, cos(t), -sin(t), 0.0f,
								0.0f, sin(t), cos(t), 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat myRotY[]=
							{ cos(t), 0, -sin(t), 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								-sin(t), 0, cos(t), 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat myRotZ[]=
								{ cos(t), -sin(t), 0.0f, 0.0f,
									sin(t), cos(t), 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f };

	//Draw Objects
  glEnable(GL_DEPTH_TEST);
  glUseProgram(program);
	mat4 scale = S(5,5,5);
	mat4 modelViewBook = T(bottompage->getPosition().x, bottompage->getPosition().y ,bottompage->getPosition().z);
	mat4 totalBook = Mult(camMatrix, Mult(modelViewBook, scale));
	//Bottom page and the back of the book
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grassTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook.m);
  DrawModel(bookback->getModel(), program, "inPosition", "inNormal", "inTexCoord");
  DrawModel(bottompage->getModel(), program, "inPosition", "inNormal", "inTexCoord");
  glUniform1f(glGetUniformLocation(program, "t"), t);

	//Top page
	glUseProgram(program);
	toppage->setPosition(upperCoord*5);
  mat4 modelViewBook2 = T(toppage->getPosition().x, toppage->getPosition().y ,toppage->getPosition().z);
  //mat4 totalBook2 = Mult(camMatrix, modelViewBook2);
  mat4 totalBook2 = Mult(camMatrix, Mult(modelViewBook2,scale));
	//totalBook2 = Mult(totalBook2, Rx(t));
  glBindTexture(GL_TEXTURE_2D, waterTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0);
	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotX"), 1, GL_TRUE, myRotX);
	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotY"), 1, GL_TRUE, myRotY);
	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotZ"), 1, GL_TRUE, myRotZ);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook2.m);
  DrawModel(toppage->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//Car

	mat4 carTot = T(car->getPosition().x, car->getPosition().y, car->getPosition().z );
	carTot = Mult(camMatrix, carTot);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, carTot.m);
	DrawModel(car->getModel(), program, "inPosition", "inNormal", "inTexCoord");

}


// Objects* Fundamentals::getObject()
// {
//     return _object;
// }
