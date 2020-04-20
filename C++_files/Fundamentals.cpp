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
	glClearColor(0.2,0.2,0.2,0);

	//init shaders
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	lampProg = loadShaders("lamp.vert", "lamp.frag");
	mainProg = loadShaders("LightSource.vert", "LightSource.frag");
	pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	programObj = loadShaders("obj.vert", "obj.frag");
	printError("load shader");

	//Load textures
	LoadTGATextureSimple("../textures/grass.tga", &grassTex);
	LoadTGATextureSimple("../textures/snow.tga", &snowTex);
	//sLoadTGATextureSimple("../textures/Paper.tga", &paperTex);
	Fundamentals::loadskybox();
	LoadTGATextureSimple("../textures/Leather2.tga", &leatherTex);
	LoadTGATextureSimple("../textures/bilskissred.tga", &bilTex);
	LoadTGATextureSimple("../textures/water.tga", &truckTex);

	//Load Models
	backModel = LoadModelPlus("../Modeller/BookBack.obj");
	carModel = LoadModelPlus("../Modeller/bilskiss.obj");
	bottomModel = LoadModelPlus("../Modeller/BookBot.obj");
	topModel = LoadModelPlus("../Modeller/BookTop.obj");
	straightPageModel = LoadModelPlus("../Modeller/PageStraight.obj");
	truckModel = LoadModelPlus("../Modeller/LPTruck.obj");
	bentPageModel =LoadModelPlus("../Modeller/PageBent.obj");
	coronaModel1 = LoadModelPlus("../Modeller/coronaSimple.obj");
	coronaModel2 =LoadModelPlus("../Modeller/coronaSimpleBase.obj");
	boxModel = LoadModelPlus("../Modeller/box.obj");
	lampModel = LoadModelPlus("../Modeller/box.obj");

	//Create Objects

	box = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, grassTex);
	lamp = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, snowTex);
	car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	coronaSimple = new Object(vec3(0.0f, 4.0f, 5.0f), coronaModel1, snowTex);
	coronaBase = new Object(vec3(5.0f, 4.0f, 0.0f), coronaModel2, grassTex);
	bookback = new Object(backPos, backModel, leatherTex);
	bottompage = new Object(bottomModel, leatherTex);
	toppage = new Object(topPos, topModel, leatherTex);
	pageStraight = new Object(straightPageModel, grassTex);
	pageBent = new Object(bentPos, bentPageModel, grassTex);
	pageBent->setTextureSide(snowTex);
	pageBent->setTextureUp(grassTex);
	pageStraight->setTextureSide(snowTex);

	book = new Book(bottompage, bookback, toppage, pageStraight, pageBent);

	//Worlds Objects
	//car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	truck = new Object(vec3(10.2f, 4.6f, 8.9f), truckModel, truckTex);
	truck->updateBoundingBox(Ry(M_PI/2), 3.0f);


	//lamp
	lampLight = new Lamp(lamp);
	lampLight -> setColour(vec3 {1.0f, 1.0f, 1.0f});
	lampColour = lampLight -> getColour();
	glUseProgram(lampProg);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform3fv(glGetUniformLocation(lampProg, "lampColour"), 1, &lampColour.x);

	///////////////////7Light////////////////////7
	//lightHandler
	lightHandler = new LightHandler();

	//pointlight
	lightPos = {1.0f, 3.0f, 0.0f};
	lightPos = lamp->getPosition();
	GLfloat constant = 1.0f;//liten betyder starkt
	GLfloat linear = 0.09;
	GLfloat quadratic = 0.012;
	vec3 lightColour = {1.0f, 0.0f, 0.0f};
	lightSource = new LightSource(lightPos, lightColour, constant, linear, quadratic);
	int index = lightHandler -> addLight(lightSource);
	vec3 colourArray = lightHandler->getColourArray();
	//printf(std::to_string(index).c_str());
	std::cout << index << std::endl;
	vec3 ambient = lightSource->getAmbient();
	vec3 diffuse = lightSource->getDiffuse();
	vec3 specular = lightSource->getSpecular();
	//lightColour =  colourArray;//lightSource->getColour();
	glUseProgram(mainProg);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.colour"), 1, &lightColour.x);
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
	dirrLight -> setColour({1.0f, 1.0f, 1.0f});
	vec3 dirrAmb = dirrLight->getAmbient();
	vec3 dirrDif = dirrLight->getDiffuse();
	vec3 dirrSpec = dirrLight->getSpecular();
	vec3 dirrDirr = dirrLight->getDirection();
	vec3 dirrColour = dirrLight->getColour();
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.ambient"), 1, &dirrAmb.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.diffuse"), 1, &dirrDif.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.specular"), 1, &dirrSpec.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.direction"), 1, &dirrDirr.x);
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.colour"), 1, &dirrColour.x);

	//spotLight
	spotLight = new LightSource(lightPos, lightColour, constant, linear, quadratic);
	spotLight -> setAmbient({0.0f, 0.0f, 0.0f});
	spotLight -> setDiffuse({1.0f, 1.0f, 1.0f});
	spotLight -> setSpecular({1.0f, 1.0f, 1.0f});
	spotLight -> setColour({1.0f, 1.0f, 1.0f});
	spotLight -> setDirection({0.0f, -1.0f, 0.0f});
	vec3 spotAmb = spotLight->getAmbient();
	vec3 spotDif = spotLight->getDiffuse();
	vec3 spotSpec = spotLight->getSpecular();
	vec3 spotDirr = spotLight->getDirection();
	vec3 spotColour = spotLight->getColour();
	GLfloat cutOff = spotLight->getCutOff();
	GLfloat outerCutOff = spotLight->getOuterCutOff();
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.ambient"), 1, &spotAmb.x);
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.diffuse"), 1, &spotDif.x);
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.specular"), 1, &spotSpec.x);
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.direction"), 1, &spotDirr.x);
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.colour"), 1, &spotColour.x);
	glUniform1f(glGetUniformLocation(mainProg, "spotLight.constant" ), constant);
	glUniform1f(glGetUniformLocation(mainProg, "spotLight.lineart" ), linear);
	glUniform1f(glGetUniformLocation(mainProg, "spotLight.quadratic" ), quadratic);
	glUniform1f(glGetUniformLocation(mainProg, "spotLight.cutOff" ), cutOff);
	glUniform1f(glGetUniformLocation(mainProg, "spotLight.outerCutOff" ), outerCutOff);

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(programObj);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(pageShader);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(pageShader, "Tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(pageShader, "TexUp"), 1);
	glUniform1i(glGetUniformLocation(pageShader, "sideTex"), 2);
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
	cameraCollisionFlag = camera->CheckCollision(coronaSimple, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(coronaBase, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(car, cameraCollisionFlag);
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
	glUniform1i(glGetUniformLocation(skyboxProg, "tex2"), 2); // Texture unit 2
	glUniform1i(glGetUniformLocation(skyboxProg, "ID"), book->getCurrentPage());
	glUniform1i(glGetUniformLocation(skyboxProg, "dir"), book->getDirection());
	glUniform1f(glGetUniformLocation(skyboxProg, "timer"), book->getTimer()/3.13);
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);

	for (unsigned int i = 0; i < 6; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skytex[i].texID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skytex[i + 6].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, skytex[i + 12].texID);

		DrawModel(skybox[i], skyboxProg, "inPosition", NULL, "inTexCoord");
	}

  glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw complete book
	book->draw(camMatrix, pageShader, t);


	//LampModel and lightsource position
	glUseProgram(lampProg);
	//vec3 newPos = {4.0f, 4.0f, 4.0f};
	//lamp->setPosition(newPos);
	mat4 scale = S(5,5,5);
	lamp->setPosition(topPos*3*sin(t));
	mat4 lampTot = T(lamp->getPosition().x, lamp->getPosition().y, lamp->getPosition().z );
	lampTot = Mult(camMatrix, Mult(scale, lampTot));
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "mdlMatrix"), 1, GL_TRUE, lampTot.m);
	DrawModel(lamp->getModel(), lampProg, "inPosition", NULL, NULL);

	//object
	glUseProgram(mainProg);
	scale = S(6,6,6);
	//pointLight
	vec3 viewPos = {camera-> getPosition().x, camera-> getPosition().y, camera-> getPosition().z};
	spotLight->setPosition(topPos*3*sin(t));
	lightPos = spotLight-> getPosition();
	//lightSource->setColour({0.2f, 0.8f*sin(t*5), 0.4f});
	//vec3 pointColour = lightSource->getColour();
	//glUniform3fv(glGetUniformLocation(mainProg, "pointLight.colour"), 1, &pointColour.x);
	//dirLight
	dirrLight->setDirection( {-0.5f, -0.5f, -0.5});
	vec3 dirrDirr = dirrLight->getDirection();
	glUniform3fv(glGetUniformLocation(mainProg, "dirLight.direction"), 1, &dirrDirr.x);
	//Box
	mat4 modelPos = T(box->getPosition().x, box->getPosition().y, box->getPosition().z);
	mat4 boxTot = Mult(camMatrix, Mult(scale, modelPos));
	//TExture
	glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grassTex);
  glUniform1i(glGetUniformLocation(mainProg, "boxTex"), 0);
	//To shader
	glUniform3fv(glGetUniformLocation(mainProg, "viewPos"), 1, &viewPos.x);
	glUniform3fv(glGetUniformLocation(mainProg, "spotLight.position"), 1, &lightPos.x);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "mdlMatrix"), 1, GL_TRUE, boxTot.m);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "model"), 1, GL_TRUE, modelPos.m);
	DrawModel(box->getModel(), mainProg, "inPosition", "inNormal", "inTexCoord");


	//draw scene
	glUseProgram(programObj);

	//Car
	if (book->getCurrentPage() == 2 ){
	mat4 modelViewCar = T(car->getPosition().x, car->getPosition().y, car->getPosition().z);
	mat4 carTot = Mult(camMatrix, modelViewCar);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, car->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, carTot.m);
	DrawModel(car->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	GLfloat coronaSimpleY = pageBent->getRealHeight(coronaSimple->getPosition().x, coronaSimple->getPosition().z);

	//std::cout << coronaSimpleY << '\n';

	mat4 modelViewCor1 = T(coronaSimple->getPosition().x, coronaSimpleY, coronaSimple->getPosition().z);
	mat4 corTot1 = Mult(camMatrix, modelViewCor1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coronaSimple->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, corTot1.m);
	DrawModel(coronaSimple->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	mat4 modelViewCor2 = T(coronaBase->getPosition().x, coronaBase->getPosition().y, coronaBase->getPosition().z);
	mat4 corTot2 = Mult(camMatrix, modelViewCor2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coronaBase->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, corTot2.m);
	DrawModel(coronaBase->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	//Truck
	mat4 modelViewTruck = T(truck->getPosition().x, truck->getPosition().y, truck->getPosition().z);
	mat4 truckTot = Mult(camMatrix, Mult(Mult(modelViewTruck, S(3,3,3)), Ry(M_PI/2)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, truck->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, truckTot.m);
	DrawModel(truck->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");
}
}

void Fundamentals::loadskybox()
{
	//glActiveTexture(GL_TEXTURE0);

	std::string	skytextures[6*3] =
	{
		"../textures/skybox0/left.tga",
		"../textures/skybox0/right.tga",
		"../textures/skybox0/top.tga",
		"../textures/skybox0/bottom.tga",
		"../textures/skybox0/front.tga",
		"../textures/skybox0/back.tga",

		"../textures/skybox2/left.tga",
		"../textures/skybox2/right.tga",
		"../textures/skybox2/top.tga",
		"../textures/skybox2/bottom.tga",
		"../textures/skybox2/front.tga",
		"../textures/skybox2/back.tga",

		"../textures/skybox1/left.tga",
		"../textures/skybox1/right.tga",
		"../textures/skybox1/top.tga",
		"../textures/skybox1/bottom.tga",
		"../textures/skybox1/front.tga",
		"../textures/skybox1/back.tga"
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

	for (unsigned int i = 0; i < 6*3; i++)
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
