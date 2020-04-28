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
#include "LightHandler.h"
#include "Book.h"
#include <iostream>
#include <list>
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
	initLights();

	//lamp

	glUseProgram(lampProg);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	//glUniform3fv(glGetUniformLocation(lampProg, "lampColour"), 1, &lampColour.x);

	///////////////////7Light////////////////////7
	//lightHandler
	pointLightVec = new LightHandler();

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(programObj);
	glUniformMatrix4fv(glGetUniformLocation(programObj, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(mainProg);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);


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
	//drawLights();
	//DRAWS THE SKYBOX
	glUseProgram(skyboxProg);
	glDisable(GL_DEPTH_TEST);
	drawSkybox();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//DRAWS THE BOOK
	book->draw(camMatrix, pageShader, t);

	//////Point Light
	glUseProgram(mainProg);
	vec3 viewPos = {camera-> getPosition().x, camera-> getPosition().y, camera-> getPosition().z};
	glUniform3fv(glGetUniformLocation(mainProg, "viewPos"), 1, &viewPos.x);


	pointLight0-> setPosition(box->getPosition());
	drawPointLight(0, pointLight0, mainProg);
	glUniform1i(glGetUniformLocation(mainProg, "number_of_point_lights"), 1);
	//För att veta vart PointLight är
  box->draw(camMatrix, mainProg, 1.0, Ry(0.0));

	///////DirLight
	dirLight0 -> setDirection(vec3(0.0f, -1.0f, 0.0f));
	drawDirLight(0, dirLight0, mainProg);
	//Måste skickas in med rätt antal
	int numDirLights = 1;
	glUniform1i(glGetUniformLocation(mainProg, "number_of_dir_lights"), numDirLights);

	///////SpotLight
	spotLight0 -> setDirection(vec3(0.0f, -1.0f, 0.0f));
	spotLight0 -> setPosition(vec3(box->getPosition().x, box->getPosition().y +10*sin(t), box->getPosition().z));
	spotLight0-> setColour(vec3(0.8f, 0.4f, 0.8f));
	spotLight0 -> setOuterCutOff(17.5);
	spotLight0 -> setCutOff(12.5);

	drawSpotLight(0, spotLight0, mainProg);
	glUniform1i(glGetUniformLocation(mainProg, "number_of_spot_lights"), 1);
	//DRAWS THE SCENES

	//glUseProgram(programObj);

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
	int list_size = listOfObj_1.size();
	for(int i = 0; i < list_size; i++){
			cameraCollisionFlag = camera->CheckCollision(listOfObj_1[i], cameraCollisionFlag);
	}
	if (book->getCurrentPage() == 2){
		int list_size = listOfObj_2.size();
		for(int i = 0; i < list_size; i++){
			cameraCollisionFlag = camera->CheckCollision(listOfObj_2[i], cameraCollisionFlag);
		}
	}
	else if (book->getCurrentPage() == 3){
		int list_size = listOfObj_3.size();
		for(int i = 0; i < list_size; i++){
			cameraCollisionFlag = camera->CheckCollision(listOfObj_3[i], cameraCollisionFlag);
		}
	}
	camera->checkFlag(cameraCollisionFlag);
	cameraCollisionFlag = false;
}

void Fundamentals::initobjects(){
	//SIMPLE OBJECTS
	// box = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, grassTex);
	// lamp = new Object(vec3(0.0f, 4.0f, 0.0f), boxModel, snowTex);
	// car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	// coronaSimple = new Object(vec3(0.0f, 4.0f, 5.0f), coronaModel1, snowTex);
	// coronaBase = new Object(vec3(5.0f, 4.0f, 0.0f), coronaModel2, grassTex);
	// car = new Object(vec3(0.0f, 4.0f, 0.0f), carModel, bilTex);
	// truck = new Object(vec3(10.2f, 4.6f, 8.9f), truckModel, waterTex);
	// truck->updateBoundingBox(Ry(M_PI/2), 3.0);


	//MULTIPLE TEXTURE OBJECTS, Object(pos, model, tex, texside, texup)
	toppage = new Object(topModel, leatherTex);
	frame = new Object(frameModel, leatherTex, leatherTex, leatherTex);
	firstPage = new Object(firstModel, grassTex, snowTex, grassTex);
	secondPage = new Object(secondModel, grassTex, snowTex, grassTex);
	pages = new Object(pagesModel, grassTex, snowTex, grassTex);
	book = new Book(toppage, firstPage, secondPage, frame, pages);
	bookMark = new Object(vec3(-14.0f, 1.0f, 0.0f), bookMarkModel, leatherTex);

	listOfObj_1.push_back(frame);
	listOfObj_1.push_back(toppage);
	listOfObj_1.push_back(firstPage);
	listOfObj_1.push_back(secondPage);
	listOfObj_1.push_back(pages);

	//OBJECTS FOR SCENE 1 obj->updateBoundingBox(Ry(angle), scale);
	house = new Object(vec3(-37.0f, 6.9f, 0.0f), houseModel, woodTex);
	house->updateBoundingBox(Ry(0), 2.0);
	cottage = new Object(vec3(-20.0f, 4.7f, -15.0f), cottageModel, woodTex);
	cottage->updateBoundingBox(Ry(M_PI), 1.0);
	cottage1 = new Object(vec3(-26.0f, 4.7f, 13.0f), cottageModel, wood2Tex);
	cottage2 = new Object(vec3(7.0f, 5.0f, 11.0f), cottageModel, cottageTex);
	cottage2->updateBoundingBox(Ry(M_PI/2), 1.0);
	elephant = new Object(vec3(-35.0f, 4.2f, 14.0f), elephantModel, leatherTex);
	elephant->updateBoundingBox(Ry(M_PI/4), 1.0);
	elephantbby = new Object(vec3(-33.3f, 3.2f, 13.0f), elephantModel, leatherTex);
	elephantbby->updateBoundingBox(Ry(7.5*M_PI/4), 0.3);
 	pile = new Object(vec3(7.0f, 3.9f, -11.0f), pileModel, woodTex);
	tree = new Object(vec3(-18.0f, 6.5f, 4.4f), treeModel, grassTex);
	tree->updateBoundingBox(Ry(0.0), 2.0);
	rosebush1 = new Object(vec3(7.0f, 4.8f, 4.4f), rosebushModel, bilTex);
	rosebush1->updateBoundingBox(Ry(0.0), 2.2);
	rosebush2 = new Object(vec3(-30.0f, 4.6f, -16.0f), rosebushModel, bilTex);
	rosebush2->updateBoundingBox(Ry(0.0), 1.5);
	rosebush3 = new Object(vec3(10.0f, 4.8f, -4.4f), rosebushModel, bilTex);
	bird = new Object(vec3(10.0f, 15.0f, -4.4f), birdModel, waterTex);
	bird2 = new Object(vec3(15.0f, 15.0f, -10.4f), birdModel, waterTex);
	bird3 = new Object(vec3(-20.0f, 15.0f, 10.4f), birdModel, waterTex);
	box = new Object(vec3(0.0f, 10.0f, 0.0f), boxModel, grassTex);


	listOfObj_2.push_back(house);
	listOfObj_2.push_back(cottage);
	listOfObj_2.push_back(cottage1);
	listOfObj_2.push_back(cottage2);
	listOfObj_2.push_back(elephant);
	listOfObj_2.push_back(elephantbby);
	listOfObj_2.push_back(pile);
	listOfObj_2.push_back(tree);
	listOfObj_2.push_back(rosebush1);
	listOfObj_2.push_back(rosebush2);
	listOfObj_2.push_back(rosebush3);
	listOfObj_2.push_back(bird);
	listOfObj_2.push_back(box);

	//OBJECTS FOR SCENE 2
	velociraptor1 = new Object(vec3(-40.0f, 3.8f, -4.0f), velociModel, leatherTex);
	velociraptor2 = new Object(vec3(-37.6f, 3.85f, -5.2f), velociModel, leatherTex);
	velociraptor3 = new Object(vec3(-37.3f, 3.8f, -1.3f), velociModel, leatherTex);
	velociraptor4 = new Object(vec3(-34.0f, 3.78f, -4.9f), velociModel, leatherTex);
	velociraptor5 = new Object(vec3(-35.0f, 3.8f, -2.0f), velociModel, leatherTex);
	velociraptor6 = new Object(vec3(-37.0f, 3.8f, -12.0f), velociModel, leatherTex);
	velociraptor7 = new Object(vec3(-40.0f, 3.8f, -8.0f), velociModel, leatherTex);
	trex = new Object(vec3(11.0f, 5.3f, 12.0f), trexModel, leatherTex);
	stegos1 = new Object(vec3(-2.0f, 4.6f, -10.0f), stegosModel, leatherTex);
	stegos2 = new Object(vec3(-5.0f, 4.6f, -16.0f), stegosModel, leatherTex);
	stegos3 = new Object(vec3(-10.0f, 4.6f, -4.0f), stegosModel, leatherTex);

	velociraptor1->updateBoundingBox(Ry(M_PI/2), 1.0);
	velociraptor2->updateBoundingBox(Ry(M_PI/2), 1.0);
	velociraptor3->updateBoundingBox(Ry(M_PI/2), 1.0);
	velociraptor4->updateBoundingBox(Ry(M_PI/2), 1.0);
	velociraptor5->updateBoundingBox(Ry(M_PI/2), 1.0);
	velociraptor6->updateBoundingBox(Ry(2*M_PI/5), 1.0);
	velociraptor7->updateBoundingBox(Ry(M_PI/3), 1.0);
	trex->updateBoundingBox(Ry(-3*M_PI/4), 1.0);
	stegos1->updateBoundingBox(Ry(M_PI/4), 1.0);
	stegos1->updateBoundingBox(Ry(-M_PI/2), 1.0);
	stegos1->updateBoundingBox(Ry(-M_PI/3), 1.0);

	listOfObj_3.push_back(velociraptor1);
	listOfObj_3.push_back(velociraptor2);
	listOfObj_3.push_back(velociraptor3);
	listOfObj_3.push_back(velociraptor4);
	listOfObj_3.push_back(velociraptor5);
	listOfObj_3.push_back(velociraptor6);
	listOfObj_3.push_back(velociraptor7);
	listOfObj_3.push_back(trex);
	listOfObj_3.push_back(stegos1);
	listOfObj_3.push_back(stegos2);
	listOfObj_3.push_back(stegos3);
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

	birdModel = LoadModelPlus("../Modeller/bird.obj");

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
	LoadTGATextureSimple("../textures/water.tga", &waterTex);
	LoadTGATextureSimple("../textures/wood.tga", &woodTex);
	LoadTGATextureSimple("../textures/wood2.tga", &wood2Tex);
	LoadTGATextureSimple("../textures/PStoTGA.tga", &greenTex);
	LoadTGATextureSimple("../textures/CottageUV.tga", &cottageTex);
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
	rosebush1->draw(camMatrix, programObj, 2.2, Ry(0.0));
	rosebush2->draw(camMatrix, programObj, 1.5, Ry(0.0));
	rosebush3->draw(camMatrix, programObj, 1.0, Ry(0.0));
	pile->draw(camMatrix, programObj, 1.0, Ry(0.0));



	mat4 modelViewbird = T(bird->getPosition().x*sin(-t), bird->getPosition().y+0.3*sin(5*t), bird->getPosition().z*cos(-t));
	mat4 Totbird = Mult(camMatrix, Mult(modelViewbird, Mult(Ry(t+4.71), Rz(3.14/3*(sin(t))))));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bird->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, Totbird.m);
	DrawModel(bird->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	mat4 mdlRotAxis = T(-15.0f, 0.0f, -10.0f);
	mat4 modelViewbird2 = T(bird2->getPosition().x*sin(-2*t), bird2->getPosition().y+0.3*sin(5*t), bird2->getPosition().z*cos(-2*t));
	mat4 rotBird2 =  Mult(mdlRotAxis, Mult(Ry(2*t+4.71), Rz(3.14/3*(sin(2*t)))));
	mat4 Totbird2 = Mult(camMatrix, Mult(modelViewbird2, rotBird2));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bird2->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, Totbird2.m);
	DrawModel(bird2->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");

	mat4 mdlRotAxis3 = T(-20.0f, 0.0f, 10.10f);
	mat4 modelViewbird3 = T(bird3->getPosition().x*sin(t), bird3->getPosition().y+2*sin(t), bird3->getPosition().z*cos(t));
	mat4 rotBird3 =  Mult(mdlRotAxis3, Mult(Ry(t+4.71), Rz(3.14/3*(sin(t)))));
	mat4 Totbird3 = Mult(camMatrix, Mult(modelViewbird3, rotBird3));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bird3->getTexture());
	glUniform1i(glGetUniformLocation(programObj, "Tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(programObj, "mdlMatrix"), 1, GL_TRUE, Totbird3.m);
	DrawModel(bird3->getModel(), programObj, "inPosition", "inNormal", "inTexCoord");





}

void Fundamentals::drawSecondScene(){

velociraptor1->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor2->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor3->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor4->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor5->draw(camMatrix, programObj, 1.0, Ry(M_PI/2));
velociraptor6->draw(camMatrix, programObj, 1.0, Ry(2*M_PI/5));
velociraptor7->draw(camMatrix, programObj, 1.0, Ry(M_PI/3));
trex->draw(camMatrix, programObj, 1.0, Ry(-3*M_PI/4));
stegos1->draw(camMatrix, programObj, 1.0, Ry(M_PI/4));
stegos2->draw(camMatrix, programObj, 1.0, Ry(-M_PI/2));
stegos3->draw(camMatrix, programObj, 1.0, Ry(-M_PI/3));
tree->draw(camMatrix, programObj, 2.0, Ry(0.0));
rosebush1->draw(camMatrix, programObj, 2.2, Ry(0.0));
rosebush2->draw(camMatrix, programObj, 1.5, Ry(0.0));
rosebush3->draw(camMatrix, programObj, 1.0, Ry(0.0));

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

void Fundamentals::drawLights(){

	glUseProgram(lampProg);
/*
	mat4 scale = S(5,5,5);
	lamp->setPosition(v*3*sin(t));
	mat4 lampTot = T(lamp->getPosition().x, lamp->getPosition().y, lamp->getPosition().z );
	lampTot = Mult(camMatrix, Mult(scale, lampTot));
	glUniformMatrix4fv(glGetUniformLocation(lampProg, "mdlMatrix"), 1, GL_TRUE, lampTot.m);
	DrawModel(lamp->getModel(), lampProg, "inPosition", NULL, NULL);*/
	//object
	glUseProgram(mainProg);
	glUniformMatrix4fv(glGetUniformLocation(mainProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 scale = S(6,6,6);
	//pointLight
	vec3 viewPos = {camera-> getPosition().x, camera-> getPosition().y, camera-> getPosition().z};
	spotLight->setPosition(v*10*sin(t));
	//pointLightVec -> setPosition(pointLightIndex, v*10*sin(t));
	//pointLightVec -> uploadPointLights(mainProg);
	lightSource->setPosition(v*10*sin(t));
	lightPos = spotLight-> getPosition();
	vec3 lightPosPoint = lightSource-> getPosition();
	glUniform3fv(glGetUniformLocation(mainProg, "pointLight.position"), 1, &lightPosPoint.x);
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

}

void Fundamentals::initLights(){
	//General

	//Point Light 0
	pointLightPos0 = {0.0f, 10.0f, 0.0f};
	pointLightColour0 = {0.9f, 0.1f, 0.1f};
	pointLight0 = new LightSource(pointLightPos0, pointLightColour0);

	//SpotLight 0
	spotLightPos0 = {5.0f, 5.0f, 5.0f};
	spotLightColour0 = {0.5f, 0.0f, 0.5f};
	spotLight0 = new LightSource(spotLightPos0, spotLightColour0);
	spotLight0 -> setAmbient(vec3(0.0f, 0.0f, 0.0f));
	spotLight0 -> setDiffuse(vec3(1.0f, 1.0f, 1.0f));
	spotLight0 -> setSpecular(vec3(1.0f, 1.0f, 1.0f));
	spotLight0 -> setLinear(1.9f);
	spotLight0 -> setConstant(1.0f);
	spotLight0 -> setQuadratic(0.032f);

	//Directional light 0
	dirLightPos0 = {10000.0f, 10.0f, 0.0f}; // KAn vara vad som helst
	dirLightColor0 =  {0.7f, 0.7f, 0.7f};
	dirLight0 = new LightSource(dirLightPos0, dirLightColor0);
	dirLight0->setDirection(vec3(0.0f, -1.0f, 0.0f));
	dirLight0 -> setAmbient(vec3(0.05f, 0.05f, 0.05f));
	dirLight0 -> setDiffuse(vec3(0.4f, 0.4f, 0.4f));
	dirLight0 -> setSpecular(vec3(0.5f, 0.5f, 0.5f));

}

void Fundamentals::drawPointLight(int index, LightSource* light, GLuint shader){

	std::string posString = "pointLightz[" + std::to_string(index) + "].position";
	std::string colString = "pointLightz[" + std::to_string(index) + "].colour";
  std::string ambString = "pointLightz[" + std::to_string(index) + "].ambient";
  std::string diffString = "pointLightz[" + std::to_string(index) + "].diffuse";
  std::string specString = "pointLightz[" + std::to_string(index) + "].specular";
  std::string constString = "pointLightz[" + std::to_string(index) + "].constant";
  std::string linString = "pointLightz[" + std::to_string(index) + "].linear";
  std::string quaString = "pointLightz[" + std::to_string(index) + "].quadratic";

	vec3 lightPos =light->getPosition();
	vec3 colour = light-> getColour();
	vec3 ambient = light-> getAmbient();
	vec3 diffuse = light-> getDiffuse();
	vec3 specular = light-> getSpecular();
	GLfloat constant = light->  getConstant();
	GLfloat linear = light->  getLinear();
	GLfloat quadratic = light ->  getQuadratic();

	glUniform3fv(glGetUniformLocation(shader, posString.c_str()), 1, &(lightPos).x);
	glUniform3fv(glGetUniformLocation(shader, colString.c_str()), 1, &(colour).x);
	glUniform3fv(glGetUniformLocation(shader, ambString.c_str()), 1, &(ambient).x);
	glUniform3fv(glGetUniformLocation(shader,  diffString.c_str()), 1, &(diffuse).x);
	glUniform3fv(glGetUniformLocation(shader,  specString.c_str()), 1, &(specular).x);
	glUniform1f(glGetUniformLocation(shader,  constString.c_str()), constant);
	glUniform1f(glGetUniformLocation(shader, linString.c_str()), linear);
	glUniform1f(glGetUniformLocation(shader, quaString.c_str()), quadratic);

}

void Fundamentals::drawDirLight(int index, LightSource* light, GLuint shader){

	std::string dirString = "dirLightz[" + std::to_string(index) + "].direction";
	std::string colString = "dirLightz[" + std::to_string(index) + "].colour";
  std::string ambString = "dirLightz[" + std::to_string(index) + "].ambient";
  std::string diffString = "dirLightz[" + std::to_string(index) + "].diffuse";
  std::string specString = "dirLightz[" + std::to_string(index) + "].specular";

	vec3 lightDir =light->getDirection();
	vec3 colour = light-> getColour();
	vec3 ambient = light-> getAmbient();
	vec3 diffuse = light-> getDiffuse();
	vec3 specular = light-> getSpecular();

	glUniform3fv(glGetUniformLocation(shader, dirString.c_str()), 1, &(lightDir).x);
	glUniform3fv(glGetUniformLocation(shader, colString.c_str()), 1, &(colour).x);
	glUniform3fv(glGetUniformLocation(shader, ambString.c_str()), 1, &(ambient).x);
	glUniform3fv(glGetUniformLocation(shader,  diffString.c_str()), 1, &(diffuse).x);
	glUniform3fv(glGetUniformLocation(shader,  specString.c_str()), 1, &(specular).x);
}


void Fundamentals::drawSpotLight(int index, LightSource* light, GLuint shader){

	std::string posString = "spotLightz[" + std::to_string(index) + "].position";
	std::string colString = "spotLightz[" + std::to_string(index) + "].colour";
	std::string dirString = "spotLightz[" + std::to_string(index) + "].direction";
  std::string ambString = "spotLightz[" + std::to_string(index) + "].ambient";
  std::string diffString = "spotLightz[" + std::to_string(index) + "].diffuse";
  std::string specString = "spotLightz[" + std::to_string(index) + "].specular";
  std::string constString = "spotLightz[" + std::to_string(index) + "].constant";
  std::string linString = "spotLightz[" + std::to_string(index) + "].linear";
  std::string quaString = "spotLightz[" + std::to_string(index) + "].quadratic";
  std::string cutOffString = "spotLightz[" + std::to_string(index) + "].cutOff";
  std::string outerCutOffString = "spotLightz[" + std::to_string(index) + "].outerCutOff";

	vec3 lightPos =light->getPosition();
	vec3 colour = light-> getColour();
	vec3 direction = light-> getDirection();
	vec3 ambient = light-> getAmbient();
	vec3 diffuse = light-> getDiffuse();
	vec3 specular = light-> getSpecular();
	GLfloat constant = light->  getConstant();
	GLfloat linear = light->  getLinear();
	GLfloat quadratic = light ->  getQuadratic();
	GLfloat cutOff = light ->  getCutOff();
	GLfloat outerCutOff = light ->  getOuterCutOff();

	glUniform3fv(glGetUniformLocation(shader, posString.c_str()), 1, &(lightPos).x);
	glUniform3fv(glGetUniformLocation(shader, colString.c_str()), 1, &(colour).x);
	glUniform3fv(glGetUniformLocation(shader, dirString.c_str()), 1, &(direction).x);
	glUniform3fv(glGetUniformLocation(shader, ambString.c_str()), 1, &(ambient).x);
	glUniform3fv(glGetUniformLocation(shader,  diffString.c_str()), 1, &(diffuse).x);
	glUniform3fv(glGetUniformLocation(shader,  specString.c_str()), 1, &(specular).x);
	glUniform1f(glGetUniformLocation(shader,  constString.c_str()), constant);
	glUniform1f(glGetUniformLocation(shader, linString.c_str()), linear);
	glUniform1f(glGetUniformLocation(shader, quaString.c_str()), quadratic);
	glUniform1f(glGetUniformLocation(shader, cutOffString.c_str()), cutOff);
	glUniform1f(glGetUniformLocation(shader, outerCutOffString.c_str()), outerCutOff);

}
