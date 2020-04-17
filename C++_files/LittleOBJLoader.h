#ifndef loadobj_h
#define loadobj_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#if defined(_WIN32)
		#include "glew.h"
	#endif
	#include <GL/gl.h>
#endif

#include "VectorUtils3.h"

// How many error messages do you want before it stops?
#define NUM_DRAWMODEL_ERROR 8

typedef struct Mtl
{
	char newmtl[255];
	
	vec3 Ka, Kd, Ks, Ke;
	// I have also seen vec3's named Tf and a float called Ni in one file. What is that?
	// Tf = transmission filter
	// Ni = optical_density
	GLfloat Ns, Tr, d; // Tr = 1-d
	int illum;	// illumination model 0..10
	char map_Ka[255], map_Kd[255], map_Ks[255], map_Ke[255], map_Ns[255], map_d[255], map_bump[255];
	
	// NEW for texture support
	int texidKa, texidKd, texidKs, texidKe, texidNs, texid_d, texid_bump; // References to loaded textures!
	// refl -type not supported -o for options? A whole bunch.
	// Extensions for physically based rendering not supported
} Mtl, *MtlPtr, **MtlHandle;

typedef struct
{
  vec3* vertexArray;
  vec3* normalArray;
  vec2* texCoordArray;
  vec3* colorArray; // Rarely used
  GLuint* indexArray;
  int numVertices;
  int numIndices;
  
  // Space for saving VBO and VAO IDs
  GLuint vao; // VAO
  GLuint vb, ib, nb, tb; // VBOs
  
  Mtl *material;
} Model;

// Basic model loading
#define LoadModelPlus LoadModel
Model* LoadModel(const char* name); // Load OBJ as single Model
Model** LoadModelSet(const char* name);  // Multi-part OBJ!

// Drawing models
void DrawModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);
void DrawWireframeModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);

// Utility functions that you may need if you want to modify the model.

Model* LoadDataToModel(
			GLfloat *vertices,
			GLfloat *normals,
			GLfloat *texCoords,
			GLfloat *colors,
			GLuint *indices,
			int numVert,
			int numInd);
void ReloadModelData(Model *m);

void CenterModel(Model *m);
void ScaleModel(Model *m, float sx, float sy, float sz);
void DisposeModel(Model *m);

#ifdef __cplusplus
}
#endif

#endif
