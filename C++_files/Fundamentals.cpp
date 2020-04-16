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
#include "Lamp.h"
#include "LightSource.h"
#include <iostream>

Fundamentals::Fundamentals(Camera* cam){
	camera = cam;
}

void Fundamentals::loadfiles(){
	void LoadTGATextureSimple(char const *filename, GLuint *tex);
	glEnable(GL_DEPTH_TEST);

	camMatrix = camera->getCamMatrix();
	projectionMatrix = camera->getProj_matrix();
	glClearColor(0.2,0.2,0.2,0);

	//init shaders
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	lampProg = loadShaders("lamp.vert", "lamp.frag");
	mainProg = loadShaders("LightSource.vert", "LightSource.frag");
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
	box = new Object();
	box->setModel(LoadModelPlus("../Modeller/box.obj"));

	//lamp
	lamp = new Object();
	lamp->setModel(LoadModelPlus("../Modeller/box.obj"));
	lampLight = new Lamp(lamp);
	lampLight -> setColour(vec3 {1.0f, 1.0f, 1.0f});
	lampColour = lampLight -> getColour();
	glUseProgram(lampProg);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform3fv(glGetUniformLocation(lampProg, "lampColour"), 1, &lampColour.x);

	//Light
	//pointlight
	lightPos = {1.0f, 3.0f, 0.0f};
	lightPos = lamp->getPosition();
	vec3 viewPos = {camera-> getPosition().x, camera-> getPosition().y, camera-> getPosition().z};
	GLfloat constant = 1.0f;
	GLfloat linear = 0.09;
	GLfloat quadratic = 0.000032;
	vec3 lightColour = {1.0f, 1.0f, 1.0f};
	lightSource = new LightSource(lightPos, lightColour, constant, linear, quadratic);
	vec3 ambient = lightSource->getAmbient();
	vec3 diffuse = lightSource->getDiffuse();
	vec3 specular = lightSource->getSpecular();
	lightColour = lightSource->getColour();
	glUseProgram(mainProg);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform3fv(glGetUniformLocation(mainProg, "lightColour"), 1, &lightColour.x);
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.ambient"), 1, &ambient.x);
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.diffuse"), 1, &diffuse.x);
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.specular"), 1, &specular.x);
	glUniform1f(glGetUniformLocation(mainProg, "pointLight.constant" ), constant);
	glUniform1f(glGetUniformLocation(mainProg, "pointLight.lineart" ), linear);
	glUniform1f(glGetUniformLocation(mainProg, "pointLight.quadratic" ), quadratic);
	printError("init shader5");

	//directional lightDi
	dirrLight = new LightSource(lightPos, lightColour, constant, linear, quadratic);
	dirrLight -> setAmbient({0.05f, 0.05f, 0.05f});
	dirrLight -> setDiffuse({0.4f, 0.4f, 0.4f});
	dirrLight -> setSpecular({0.5f, 0.5f, 0.5f});
	vec3 dirrAmb = dirrLight->getAmbient();
	vec3 dirrDif = dirrLight->getDiffuse();
	vec3 dirrSpec = dirrLight->getSpecular();
	vec3 dirrDirr = dirrLight->getDirection();
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.ambient"), 1, &dirrAmb.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.diffuse"), 1, &dirrDif.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.specular"), 1, &dirrDif.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.direction"), 1, &dirrDirr.x);

	//SkyBox
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
  //DrawModel(skybox, skyboxProg, "in_Position", NULL, "inTexCoord");


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
  //DrawModel(bookback->getModel(), program, "inPosition", "inNormal", "inTexCoord");
  //DrawModel(bottompage->getModel(), program, "inPosition", "inNormal", "inTexCoord");
  glUniform1f(glGetUniformLocation(program, "t"), t);

	//Top page
	toppage->setPosition(upperCoord*5);
  mat4 modelViewBook2 = T(toppage->getPosition().x, toppage->getPosition().y ,toppage->getPosition().z);
  mat4 totalBook2 = Mult(camMatrix, modelViewBook2);
  totalBook2 = Mult(camMatrix, Mult(modelViewBook2,scale));
  glBindTexture(GL_TEXTURE_2D, waterTex);
  glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook2.m);
  //DrawModel(toppage->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//Car
	mat4 carTot = T(car->getPosition().x, car->getPosition().y, car->getPosition().z );
	carTot = Mult(camMatrix, carTot);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, carTot.m);
	//DrawModel(car->getModel(), program, "inPosition", "inNormal", "inTexCoord");

	//LampModel and lightsource position
	glUseProgram(lampProg);
	//vec3 newPos = {4.0f, 4.0f, 4.0f};
	//lamp->setPosition(newPos);
	scale = S(5,5,5);
	lamp->setPosition(upperCoord*3*sin(t));
	mat4 lampTot = T(lamp->getPosition().x, lamp->getPosition().y, lamp->getPosition().z );
	lampTot = Mult(camMatrix, Mult(scale, lampTot));
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "mdlMatrix"), 1, GL_TRUE, lampTot.m);
	DrawModel(lamp->getModel(), lampProg, "inPosition", NULL, NULL);

	//object
	glUseProgram(mainProg);
	scale = S(6,6,6);
	vec3 viewPos = {camera-> getPosition().x, camera-> getPosition().y, camera-> getPosition().z};
	lightSource->setPosition(upperCoord*3*sin(t));
	lightPos = lightSource-> getPosition();
	mat4 modelPos = T(box->getPosition().x, box->getPosition().y, box->getPosition().z);
	mat4 boxTot = Mult(camMatrix, Mult(scale, modelPos));
	//TExture
	glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grassTex);
  glUniform1i(glGetUniformLocation(mainProg, "boxTex"), 0);
	//To shader
	glUniform3fv(glGetUniformLocation(mainProg, "viewPos"), 1, &viewPos.x);
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.position"), 1, &lightPos.x);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "mdlMatrix"), 1, GL_TRUE, boxTot.m);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "model"), 1, GL_TRUE, modelPos.m);
	DrawModel(box->getModel(), mainProg, "inPosition", "inNormal", "inTexCoord");

}


// Objects* Fundamentals::getObject()
// {
//     return _object;
// }
