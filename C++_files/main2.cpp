
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

// Globals
// Data would normally be read from files
#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

Model *boktop;
Model *bokrygg;
Model *skybox;

mat4 rot, trans, total, rot2, trans2, total2, totalBook, totalBook2;
mat4 projectionMatrix2;

mat4 camMat, camMat2;

vec3 p,l, c, cam, lookAtPoint, c2;
LoadFiles* loader;

void LoadTGATextureSimple(char *filename, GLuint *tex);

GLuint program, skyboxProg, pageShader;
GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
unsigned int bunnyTex;
unsigned int teddyTex;
GLfloat viewX = 0.5;
GLfloat viewY = 0.5;
vec3 v = {0.0, 1.0, 0.0};
GLfloat Bookx = 0.0f;
GLfloat Bookz = 0.0f;
GLfloat Booky = 0.0f;


void mouseControl(int x, int y)
{

	loader->getMouse(x,y);
	// viewX = (float)x/600*2*M_PI;
	// viewY = (float)y/600*M_PI;
	//
	// lookAtPoint.x = -10 *sin(viewY)*sin(viewX) + cam.x;
	// lookAtPoint.y = 10*cos(viewY) + cam.y;
	// lookAtPoint.z = 10 *sin(viewY)*cos(viewX) + cam.z;

	glutPostRedisplay();
}

// void keyboardInput(){
// 			vec3 forward = VectorSub(cam, lookAtPoint);
//
// 	if (glutKeyIsDown('w')) {
// 		c = Normalize(forward);
// 		c = ScalarMult(c,0.5);
// 		cam = VectorSub(cam, c);
// 		lookAtPoint = VectorSub(lookAtPoint, c);
// 	 }
// 	 if (glutKeyIsDown('s')) {
// 		 c = Normalize(forward);
// 		 c = ScalarMult(c,0.5);
// 		cam = VectorAdd(cam, c);
// 		lookAtPoint = VectorAdd(lookAtPoint, c);
// 		}
// 		if (glutKeyIsDown('a')) {
// 			c = CrossProduct(v, forward);
// 			c = Normalize(c);
// 			c = ScalarMult(c,0.5);
// 			cam = VectorSub(cam, c);
// 			lookAtPoint = VectorSub(lookAtPoint, c);
// 		}
// 	 if (glutKeyIsDown('d')) {
// 			 c = CrossProduct(v, forward);
// 			 c = Normalize(c);
// 			 c = ScalarMult(c,0.5);
// 			 cam = VectorAdd(cam, c);
// 			 lookAtPoint = VectorAdd(lookAtPoint, c);
// 	 }
// }

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


void init(void)
{
	loader = new LoadFiles();
	loader->initiate();


  // //Cam init
	// cam = SetVector(0, 5, 8);
	// lookAtPoint = SetVector(0,0,0);
	// v = SetVector(0,1,0);
	// projectionMatrix2 = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);
	//
	// // GL inits
	// glClearColor(0.9,0.9,1,0);
	// glEnable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
	// printError("GL inits");
	//
	// // Load and compile shader
	// program = loadShaders("terrain.vert", "terrain.frag");
	// skyboxProg = loadShaders("sky.vert", "sky.frag");
	// pageShader = loadShaders("pageShader.vert", "pageShader.frag");
	// glUseProgram(program);
	// printError("init shader");
	// glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix2.m);
	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "projMatrix"), 1, GL_TRUE, projectionMatrix2.m);
	//
  // LoadTGATextureSimple("../textures/grass.tga", &grassTex);
  // LoadTGATextureSimple("../textures/snow.tga", &snowTex);
  // LoadTGATextureSimple("../textures/water.tga", &waterTex);
  // LoadTGATextureSimple("../textures/SkyBox512.tga", &skytex);
	//
  // skybox = LoadModelPlus("../Modeller/skybox.obj");
  // boktop = LoadModelPlus("../Modeller/Boktop.obj");
  // bokrygg = LoadModelPlus("../Modeller/bokrygg.obj");
	//loader->loadFiles();
}

void OnTimer(int value)
{
		glutPostRedisplay();
		glutTimerFunc(20, &OnTimer, value);
}

void display(void)
{

	loader->update();
	loader->keyboardInput();
		//keyboardInput();
//   GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
// 	t = t/1000;
// 	// clear the screen
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glDisable(GL_CULL_FACE);
//
// 	printError("pre display");
// 	mat4 camMatrix = lookAt(cam.x, cam.y, cam.z,
// 										lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
// 										v.x, v.y, v.z);
// 	mat4 modelView = IdentityMatrix();
// 	mat4 total = Mult(camMatrix, modelView);
// 	mat4 camMat2 = camMatrix;
// 	camMat2.m[3] = 0;
// 	camMat2.m[7] = 0;
// 	camMat2.m[11] = 0;
// 	camMat2.m[15] = 1;
//
//
// 	glUseProgram(skyboxProg);
// 	glDisable(GL_DEPTH_TEST);
// 	glBindTexture(GL_TEXTURE_2D, skytex);
// 	glUniform1i(glGetUniformLocation(skyboxProg, "texUnit"), 0); // Texture unit 0
// 	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix2.m);
// 	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);
// 	DrawModel(skybox, skyboxProg, "in_Position", NULL, "inTexCoord");
// 	//loader->drawsky();
//
// //Initiate rotation matrices
// 	GLfloat myRotX[]=
// 							{ 1.0f, 0.0f, 0.0f, 0.0f,
// 								0.0f, cos(t), -sin(t), 0.0f,
// 								0.0f, sin(t), cos(t), 0.0f,
// 								0.0f, 0.0f, 0.0f, 1.0f };
//
// 	GLfloat myRotY[]=
// 							{ cos(t), 0, sin(t), 0.0f,
// 								0.0f, 1.0f, 0.0f, 0.0f,
// 								-sin(t), 0, cos(t), 0.0f,
// 								0.0f, 0.0f, 0.0f, 1.0f };
//
// 	GLfloat myRotZ[]=
// 								{ cos(t), -sin(t), 0.0f, 0.0f,
// 									sin(t), cos(t), 0.0f, 0.0f,
// 									0.0f, 0.0f, 1.0f, 0.0f,
// 									0.0f, 0.0f, 0.0f, 1.0f };
//
//
// //Camera vectors
// 	glEnable(GL_DEPTH_TEST);
// 	mat4 scale = S(5,5,5);
//   mat4 modelViewBook = T(Bookx, Booky, Bookz);
//   mat4 totalBook = Mult(camMatrix, Mult(modelViewBook, scale));
//
//   glUseProgram(program);
// 	//Draw the bunny
//   glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, grassTex);
// 	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
// 	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook.m);
// 	DrawModel(boktop, program, "inPosition", "inNormal", "inTexCoord");
//   DrawModel(bokrygg, program, "inPosition", "inNormal", "inTexCoord");
// 	glUniform1f(glGetUniformLocation(program, "t"), t);
//
// 	GLfloat upperPage = Booky+2.5*5;
// 	mat4 modelViewBook2 = T(Bookx, upperPage, Bookz);
// 	mat4 totalBook2 = Mult(camMatrix, modelViewBook2);
// 	totalBook2 = Mult(camMatrix, Mult(modelViewBook2,scale));
//
// 	glBindTexture(GL_TEXTURE_2D, waterTex);
// 	glUniform1i(glGetUniformLocation(program, "bookTex"), 0); // Texture unit 0
// 	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBook2.m);
// 	DrawModel(boktop, program, "inPosition", "inNormal", "inTexCoord");
//
// 	// glUseProgram(pageShader);
//   // glBindTexture(GL_TEXTURE_2D, waterTex);
// 	// glUniform1i(glGetUniformLocation(pageShader, "bookTex"), 0); // Texture unit 0
// 	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotY"), 1, GL_TRUE, myRotY);
// 	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotX"), 1, GL_TRUE, myRotX);
// 	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "myRotZ"), 1, GL_TRUE, myRotZ);
// 	// glUniformMatrix4fv(glGetUniformLocation(pageShader, "mdlMatrix"), 1, GL_TRUE, totalBook2.m);
// 	// DrawModel(boktop, pageShader, "inPosition", "inNormal", "inTexCoord");
//
 	printError("display");
 	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("GLUTen project");
	glutDisplayFunc(display);
#ifdef WIN32
	glewInit();
#endif
	init ();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouseControl);
	glutMainLoop();
	exit(0);
}
