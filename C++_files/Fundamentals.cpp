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

	//INIT MATRICES
	camMatrix = camera->getCamMatrix();
	projectionMatrix = camera->getProj_matrix();

	//INIT
	initshaders();
	loadtextures();
	loadmodels();
	initobjects();

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(programObj);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(pageShader);
	glUniformMatrix4fv(glGetUniformLocation(pageShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(pageShader, "Tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(pageShader, "TexUp"), 1);
	glUniform1i(glGetUniformLocation(pageShader, "sideTex"), 2);

	glUseProgram(skyboxProg);
	glUniform1i(glGetUniformLocation(skyboxProg, "tex0"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(skyboxProg, "tex1"), 1); // Texture unit 1
	glUniform1i(glGetUniformLocation(skyboxProg, "tex2"), 2); // Texture unit 1
	printError("init shader");
}


void Fundamentals::update(){
	//CLEAR THE SCREEN
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	//UPDATE THE CAMERA
	cameraCollision();
	camMatrix = camera->getCamMatrix();
	camMat2 = camMatrix;
	camMat2.m[3] = 0;
	camMat2.m[7] = 0;
	camMat2.m[11] = 0;
	camMat2.m[15] = 1;

  t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
  printError("pre display");

	//DRAWS THE SKYBOX
	glUseProgram(skyboxProg);
	glDisable(GL_DEPTH_TEST);
	drawSkybox();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//DRAWS THE BOOK
	book->draw(camMatrix, pageShader, t);

	//DRAWS THE SCENES
	glUseProgram(programObj);
	if (book->getCurrentPage() == 1 && book->getFadeBool()){
				book->setFadeBool();
			}

	else if (book->getCurrentPage() == 2){
		if (book->getFadeBool()){
					fadeOutObjects();
			}
			else{
				fadeInObjects();
			}
			drawFirstScene();
		}

		else if (book->getCurrentPage() == 3){
			if (book->getFadeBool()){
						fadeOutObjects();
					}
					else{
						fadeInObjects();
					}
				drawSecondScene();
		}
}

void Fundamentals::cameraCollision(){
	// Book
	cameraCollisionFlag = camera->CheckCollision(pages, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(frame, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(toppage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(firstPage, cameraCollisionFlag);
	cameraCollisionFlag = camera->CheckCollision(secondPage, cameraCollisionFlag);

	//object
	camera->checkFlag(cameraCollisionFlag);
	cameraCollisionFlag = false;
}

void Fundamentals::initobjects(){
	//SIMPLE OBJECTS
	box = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, grassTex);
	lamp = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, snowTex);
	car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	coronaSimple = new Object(vec3(0.0f, 4.0f, 5.0f), coronaModel1, snowTex);
	coronaBase = new Object(vec3(5.0f, 4.0f, 0.0f), coronaModel2, grassTex);
	car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	truck = new Object(vec3(10.2f, 4.6f, 8.9f), truckModel, truckTex);
	truck->updateBoundingBox(Ry(M_PI/2), 3.0);
	toppage = new Object(topModel, leatherTex);

	//MULTIPLE TEXTURE OBJECTS, Object(pos, model, tex, texside, texup)
	frame = new Object(frameModel, leatherTex, leatherTex, leatherTex);
	firstPage = new Object(firstModel, grassTex, snowTex, grassTex);
	secondPage = new Object(secondModel, grassTex, snowTex, grassTex);
	pages = new Object(pagesModel, waterTex, grassTex, snowTex);
	book = new Book(toppage, firstPage, secondPage, frame, pages);
	bookMark = new Object(vec3(-14.0f, 1.0f, 0.0f), bookMarkModel, leatherTex);

	//OBJECTS FOR SCENE 1
	house = new Object(vec3(-37.0f, 6.9f, 0.0f), houseModel, woodTex);
	cottage = new Object(vec3(-20.0f, 4.7f, -15.0f), cottageModel, woodTex);
	cottage1 = new Object(vec3(-26.0f, 4.7f, 13.0f), cottageModel, greenTex);
	cottage2 = new Object(vec3(7.0f, 5.0f, 11.0f), cottageModel, wood2Tex);
	elephant = new Object(vec3(-35.0f, 4.2f, 14.0f), elephantModel, leatherTex);
	elephantbby = new Object(vec3(-33.3f, 3.2f, 13.0f), elephantModel, leatherTex);
 	pile = new Object(vec3(7.0f, 3.9f, -11.0f), pileModel, woodTex);
	tree = new Object(vec3(-18.0f, 6.5f, 4.4f), treeModel, grassTex);
	rosebush = new Object(vec3(7.0f, 4.8f, 4.4f), rosebushModel, bilTex);

	//OBJECTS FOR SCENE 2
	velociraptor1 = new Object(vec3(-40.0f, 3.8f, -4.0f), velociModel, leatherTex);
	velociraptor2 = new Object(vec3(-37.6f, 3.85f, -5.2f), velociModel, leatherTex);
	velociraptor3 = new Object(vec3(-37.3f, 3.8f, -3.3f), velociModel, leatherTex);
	velociraptor4 = new Object(vec3(-34.0f, 3.78f, -4.9f), velociModel, leatherTex);
	velociraptor5 = new Object(vec3(-35.0f, 3.8f, -3.0f), velociModel, leatherTex);
	trex = new Object(vec3(4.0f, 5.3f, -4.0f), trexModel, leatherTex);
	stegos = new Object(vec3(7.0f, 5.6f, -14.0f), stegosModel, leatherTex);
}

void Fundamentals::loadmodels(){
	topModel = LoadModelPlus("../Modeller/booktopreal.obj");
	firstModel = LoadModelPlus("../Modeller/pagefirst.obj");
	secondModel =LoadModelPlus("../Modeller/pagesecond.obj");
	frameModel = LoadModelPlus("../Modeller/bookstaticcover.obj");
	pagesModel = LoadModelPlus("../Modeller/bookstaticpages.obj");
	coronaModel1 = LoadModelPlus("../Modeller/coronaSimple.obj");
	coronaModel2 = LoadModelPlus("../Modeller/coronaSimpleBase.obj");
	carModel = LoadModelPlus("../Modeller/bilskiss.obj");
	truckModel = LoadModelPlus("../Modeller/LPTruck.obj");
	boxModel = LoadModelPlus("../Modeller/box.obj");
	lampModel = LoadModelPlus("../Modeller/box.obj");
	bookMarkModel = LoadModelPlus("../Modeller/bookmark.obj");

	//MODELS FOR SCENE1
	houseModel = LoadModelPlus("../Modeller/house3.obj");
	cottageModel = LoadModelPlus("../Modeller/cottage.obj");
	elephantModel = LoadModelPlus("../Modeller/elephant.obj");
	treeModel = LoadModelPlus("../Modeller/tree.obj");
	rosebushModel = LoadModelPlus("../Modeller/rosebush.obj");
	pileModel = LoadModelPlus("../Modeller/pile.obj");

	//MODELS FOR SCENE2
	velociModel = LoadModelPlus("../Modeller/velociraptor.obj");
	trexModel = LoadModelPlus("../Modeller/trex.obj");
	stegosModel = LoadModelPlus("../Modeller/stegosaurus.obj");
}

void Fundamentals::loadtextures(){
	LoadTGATextureSimple("../textures/grass.tga", &grassTex);
	LoadTGATextureSimple("../textures/snow.tga", &snowTex);
	loadskybox();
	LoadTGATextureSimple("../textures/Leather2.tga", &leatherTex);
	LoadTGATextureSimple("../textures/bilskissred.tga", &bilTex);
	LoadTGATextureSimple("../textures/water.tga", &truckTex);
	LoadTGATextureSimple("../textures/wood.tga", &woodTex);
	LoadTGATextureSimple("../textures/wood2.tga", &wood2Tex);
	LoadTGATextureSimple("../textures/PStoTGA.tga", &greenTex);
}

void Fundamentals::initshaders(){
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	lampProg = loadShaders("lamp.vert", "lamp.frag");
	mainProg = loadShaders("LightSource.vert", "LightSource.frag");
	pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	programObj = loadShaders("obj.vert", "obj.frag");
	printError("load shader");
}

void Fundamentals::fadeOutObjects(){
	GLfloat timer = getMyTimer();
	glUniform1f(glGetUniformLocation(programObj, "timer"), timer);
	decreaseMyTimer(0.01);

	if (timer <= 0){
		book -> setFadeBool();
	}
}

void Fundamentals::fadeInObjects(){
	if (!book->getRotationBool()){
		GLfloat timer = getMyTimer();
		timer = timer;
		glUniform1f(glGetUniformLocation(programObj, "timer"), timer);

		if (timer <= 1){
			increaseMyTimer(0.01);
		}
	}
	else {
		setMyTimer(0.0);
	}
}


void Fundamentals::loadskybox(){
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	};

	for (unsigned int i = 0; i < 6; i++)
	{
		skybox[i] = LoadModelPlus(filename[i].c_str());
	}
}


void Fundamentals::drawFirstScene(){
	bookMark->draw(camMatrix, programObj, 1.0, Ry(0.0));

	house->draw(camMatrix, programObj, 2.0, Ry(0.0)); // camMatrix, shader, scale, angle
	cottage->draw(camMatrix, programObj, 1.0, Ry(M_PI));
	cottage1->draw(camMatrix, programObj, 1.0, Ry(0.0));
	cottage2->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
	elephant->draw(camMatrix, programObj, 1.0, Ry(M_PI/4));
	elephantbby->draw(camMatrix, programObj, 0.3, Ry(7.5*M_PI/4));
	tree->draw(camMatrix, programObj, 2.0, Ry(0.0));
	rosebush->draw(camMatrix, programObj, 1.0, Ry(0.0));
	pile->draw(camMatrix, programObj, 1.0, Ry(0.0));

}

void Fundamentals::drawSecondScene(){
velociraptor1->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor2->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor3->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor4->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor5->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
trex->draw(camMatrix, programObj, 1.0, Ry(M_PI/4));
stegos->draw(camMatrix, programObj, 1.0, Ry(-M_PI/4));
tree->draw(camMatrix, programObj, 2.0, Ry(0.0));
}


void Fundamentals::drawSkybox(){
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
}
