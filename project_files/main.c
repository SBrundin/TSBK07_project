// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix, totalSphere, transSphere, rotSphere, camMat2;

vec3 c, v, cam, lookAtPoint, c2;

GLfloat viewX, viewY, viewZ, a;

GLfloat Ballx = 0.0f;
GLfloat Ballz = 0.0f;


GLfloat viewX = 0.5;
GLfloat viewY = 0.5;
vec3 v = {0.0, 1.0, 0.0};

GLuint sphereTex, waterTex, snowTex;
//vec3 cam = {0, 5, 8};
GLfloat	BallT;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	vec3 normalVec, u,v;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 5.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

			normalArray[(x + z * tex->width)*3 + 0] = 0;
			normalArray[(x + z * tex->width)*3 + 1] = 1;
			normalArray[(x + z * tex->width)*3 + 2] = 0;

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;

			if (x>0 && z>0){
			u.x = vertexArray[((x+1) + z * tex->width)*3 + 0] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 0];
			u.y = vertexArray[((x+1) + z * tex->width)*3 + 1] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 1];
			u.z = vertexArray[((x+1) + z * tex->width)*3 + 2] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 2];

			v.x = vertexArray[(x + (z+1) * tex->width)*3 + 0] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 0];
			v.y = vertexArray[(x + (z+1)  * tex->width)*3 + 1] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 1];
			v.z = vertexArray[(x + (z+1)  * tex->width)*3 + 2] - vertexArray[((x-1) + (z-1) * tex->width)*3 + 2];

			normalVec = CrossProduct(v, u);

			normalArray[(x + z * tex->width)*3 + 0] = normalVec.x;
			normalArray[(x + z * tex->width)*3 + 1] = normalVec.y;
			normalArray[(x + z * tex->width)*3 + 2] = normalVec.z;
		}
	}

	// End of terrain generation
	// Create Model and upload to GPU:
	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);
	return model;
}

// vertex array object
Model *m, *m2, *tm, *sphere, *skybox;
// Reference to shader program
GLuint program, proPhong, skyboxProg, skytex;
GLuint tex1, tex2, sphereTex;
TextureData ttex; // terrain

void mouse(int x, int y)
{
	//printf("%d %d\n", x, y);
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

GLfloat getCorrHeightInt(int x, int z, GLfloat *vertexArray, TextureData *tex){
	return vertexArray[(x + z * tex->width)*3 + 1];
}

GLfloat getRealHeight(GLfloat x, GLfloat z, GLfloat *vertexArray, TextureData *tex){
		GLfloat p1,p2,p3,p4,height,u,v,uPrim,vPrim;
		p1 = getCorrHeightInt(floor(x), ceil(z), vertexArray, tex);
		p2 = getCorrHeightInt(ceil(x), ceil(z), vertexArray, tex);
		p3 = getCorrHeightInt(floor(x), floor(z), vertexArray, tex);
		p4 = getCorrHeightInt(ceil(x), floor(z), vertexArray, tex);
		height = 0.0f;
		u = x-floor(x);
		uPrim = 1-u;
		v = z-floor(z);
		vPrim = 1-v;
		height = v*(uPrim*p1 + u*p2) + vPrim*(uPrim*p3 + u*p4);
		return height;
	}

void init(void)
{
	cam = SetVector(0, 5, 8);
	lookAtPoint = SetVector(0,0,0);
	v = SetVector(0,1,0);

	// GL inits
	glClearColor(0.9,0.9,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	//proPhong = loadShaders("terrain.vert", "terrain.frag");
	skyboxProg = loadShaders("sky.vert", "sky.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
//	glUniformMatrix4fv(glGetUniformLocation(proPhong, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "snowTex"), 1);
	glUniform1i(glGetUniformLocation(program, "waterTex"), 2);
	LoadTGATextureSimple("../textures/grass.tga", &tex1);
	LoadTGATextureSimple("../textures/snow.tga", &snowTex);
	LoadTGATextureSimple("../textures/water.tga", &waterTex);
	LoadTGATextureSimple("../textures/SkyBox512.tga", &skytex);
	skybox = LoadModelPlus("../textures/skybox.obj");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, snowTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterTex);
// Load terrain data
	LoadTGATextureData("../textures/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	sphere = LoadModelPlus("../textures/octagon.obj");
	LoadTGATextureSimple("../textures/maskros512.tga", &sphereTex);

	printError("init terrain");
}

void display(void)
{

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	t = t/1000;
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	mat4 total, modelView, camMatrix;
	printError("pre display");
	//camMat2 = mat3tomat4(mat4tomat3(camMat)); //worldToView
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
	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProg, "mdlMatrix"), 1, GL_TRUE, camMat2.m);
	DrawModel(skybox, skyboxProg, "in_Position", NULL, "inTexCoord");


	// Build matrix
	keyboardInput();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, snowTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	glUniform1f(glGetUniformLocation(program, "t"), t);

// Draw sphere
	//transSphere =
 	a += M_PI/90;
	Ballx = 25+cos(a);
	Ballz = 15+sin(a);

	GLfloat rull = getRealHeight(Ballx, Ballz, tm->vertexArray, &ttex);

	modelView = T(Ballx,rull, Ballz);

	totalSphere = Mult(camMatrix, modelView);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalSphere.m);
	DrawModel(sphere, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
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
