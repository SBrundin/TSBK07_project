// // Lab 4, terrain generation
//
// #ifdef __APPLE__
// 	#include <OpenGL/gl3.h>
// 	// Linking hint for Lightweight IDE
// 	// uses framework Cocoa
// #endif
// #include "MicroGlut.h"
// #include "GL_utilities.h"
// #include "VectorUtils3.h"
// #include "loadobj.h"
// #include "LoadTGA.h"
//
//
// int main(int argc, char **argv)
// {
// 	glutInit(&argc, argv);
// 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
// 	glutInitContextVersion(3, 2);
// 	glutInitWindowSize (600, 600);
// 	glutCreateWindow ("GLUTen project");
// 	glutMainLoop();
// 	exit(0);
// }

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

mat4 rot, trans, total, rot2, trans2, total2;
mat4 projectionMatrix2;

mat4 camMat, camMat2;

vec3 p,l, c, cam, lookAtPoint, c2;;

void LoadTGATextureSimple(char *filename, GLuint *tex);

GLuint program, skyboxProg;
GLuint sphereTex, waterTex, snowTex, skytex,grassTex, tex2, bookTex;
unsigned int bunnyTex;
unsigned int teddyTex;
GLfloat viewX = 0.5;
GLfloat viewY = 0.5;
vec3 v = {0.0, 1.0, 0.0};


void mouse(int x, int y)
{
	viewX = (float)x/600*2*M_PI;
	viewY = (float)y/600*M_PI;

	lookAtPoint.x = -10 *sin(viewY)*sin(viewX) + cam.x;
	lookAtPoint.y = 10*cos(viewY) + cam.y;
	lookAtPoint.z = 10 *sin(viewY)*cos(viewX) + cam.z;

	glutPostRedisplay();
}

void keyboardInput(){
			vec3 forward = VectorSub(cam, lookAtPoint);

	if (glutKeyIsDown('w')) {
		c = Normalize(forward);
		c = ScalarMult(c,0.5);
		cam = VectorSub(cam, c);
		lookAtPoint = VectorSub(lookAtPoint, c);
	 }
	 if (glutKeyIsDown('s')) {
		 c = Normalize(forward);
		 c = ScalarMult(c,0.5);
		cam = VectorAdd(cam, c);
		lookAtPoint = VectorAdd(lookAtPoint, c);
		}
		if (glutKeyIsDown('a')) {
			c = CrossProduct(v, forward);
			c = Normalize(c);
			c = ScalarMult(c,0.5);
			cam = VectorSub(cam, c);
			lookAtPoint = VectorSub(lookAtPoint, c);
		}
	 if (glutKeyIsDown('d')) {
			 c = CrossProduct(v, forward);
			 c = Normalize(c);
			 c = ScalarMult(c,0.5);
			 cam = VectorAdd(cam, c);
			 lookAtPoint = VectorAdd(lookAtPoint, c);
	 }
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void init(void)
{
  //Cam init
	cam = SetVector(0, 5, 8);
	lookAtPoint = SetVector(0,0,0);
	v = SetVector(0,1,0);
	projectionMatrix2 = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);

	// GL inits
	glClearColor(0.9,0.9,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	glUseProgram(program);
	printError("init shader");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix2.m);
  glUniform1i(glGetUniformLocation(program, "tex"), 0);

  LoadTGATextureSimple("../textures/grass.tga", &grassTex);
  LoadTGATextureSimple("../textures/snow.tga", &snowTex);
  LoadTGATextureSimple("../textures/water.tga", &waterTex);
  LoadTGATextureSimple("../textures/SkyBox512.tga", &skytex);

  skybox = LoadModelPlus("../Modeller/skybox.obj");
  boktop = LoadModelPlus("../Modeller/Boktop.obj");
  bokrygg = LoadModelPlus("../Modeller/bokrygg.obj");
}

void OnTimer(int value)
{
		glutPostRedisplay();
		glutTimerFunc(20, &OnTimer, value);
}

void display(void)
{
  keyboardInput();
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	t = t/1000;
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	mat4 total, modelView, camMatrix;
	printError("pre display");
	camMatrix = lookAt(cam.x, cam.y, cam.z,
										lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
										v.x, v.y, v.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	camMat2 = camMatrix;
	camMat2.m[3] = 0;
	camMat2.m[7] = 0;
	camMat2.m[11] = 0;
	camMat2.m[15] = 1;

	//Skybox with corr program
	glUseProgram(skyboxProg);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, skytex);
	glUniform1i(glGetUniformLocation(skyboxProg, "texUnit"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix2.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);
	DrawModel(skybox, skyboxProg, "in_Position", NULL, "inTexCoord");

//Initiate rotation matrices
	// GLfloat myRotX[]=
	// 						{ 1.0f, 0.0f, 0.0f, 0.0f,
	// 							0.0f, cos(t), -sin(t), 0.0f,
	// 							0.0f, sin(t), cos(t), 0.0f,
	// 							0.0f, 0.0f, 0.0f, 1.0f };
  //
	// GLfloat myRotY[]=
	// 						{ cos(t), 0, sin(t), 0.0f,
	// 							0.0f, 1.0f, 0.0f, 0.0f,
	// 							-sin(t), 0, cos(t), 0.0f,
	// 							0.0f, 0.0f, 0.0f, 1.0f };
  //
	// GLfloat myRotZ[]=
	// 							{ cos(t), -sin(t), 0.0f, 0.0f,
	// 								sin(t), cos(t), 0.0f, 0.0f,
	// 								0.0f, 0.0f, 1.0f, 0.0f,
	// 								0.0f, 0.0f, 0.0f, 1.0f };

//Camera vectors
	glEnable(GL_DEPTH_TEST);

//p = SetVector(4, 3, 3);
// p = SetVector(5*cos(t), 3, 5*sin(t));
// l = SetVector(0, 5, 0);
// v = SetVector(0, 1, 0);
//
// camMat = lookAtv(p, l, v); //worldToView
//
// trans = T(1.4, 0, 1.2);
// rot = Ry(40);
// total = Mult(rot, trans); //modelToWorld
//
// trans2 = T(-1.0, 0, -1.9);
// rot2 = Ry(40);
// total2 = Mult(rot2, trans2); //modelToWorld2

  glUseProgram(program);
	//Draw the bunny
  glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bunnyTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(boktop, program, "inPosition", "inNormal", "inTexCoord");

	// //Draw the teddy
	// glBindTexture(GL_TEXTURE_2D, teddyTex);
	// glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	// glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	// DrawModel(n, program, "inPosition", "inNormal", "inTexCoord");

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
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
